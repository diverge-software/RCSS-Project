/*********************************************************************
*
*   MODULE NAME:
*       udp_client.cpp - UDP Client Processing
*
*   DESCRIPTION:
*       Performs UDP interface processing
*
*---------------------------------------------------------------------
* $Id: udp_client.cpp, v1.0, 2011-09-23 17:25:00Z, Joseph Wachtel$
* $NoKeywords$
*********************************************************************/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <iostream>
#include <sstream>
#include <string>

#include "udp_client.hpp"
#include "udp_client_q.hpp"

using namespace std;

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           PROJECT INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

const string            UDP_client::udp_SERVER_IP = "192.168.1.3";
                                    /* UDP server IP                */
const unsigned          UDP_client::udp_SERVER_PORT = 6000;
                                    /* UDP server port              */
const unsigned          UDP_client::udp_SERVER_PKT_SIZE = 8192;
                                    /* UDP server packet size       */

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/


/*********************************************************************
*
*   PROCEDURE NAME:
*       ~UDP_client - UDP Client Destructor
*
*   DESCRIPTION:
*       Class destructor for UDP client
*
*********************************************************************/

UDP_client::~UDP_client             /* UDP Client Destructor        */
    ( void )                             
{
/*----------------------------------------------------------
Close open UDP socket
----------------------------------------------------------*/
if( this->udp_client_cb.socket_open )
    {
    closesocket( this->udp_skt_fd );
    this->udp_client_cb.socket_open = FALSE;
    }

/*----------------------------------------------------------
Terminate receive thread if running
----------------------------------------------------------*/
if( this->udp_client_cb.rx_thrd_alive )
    {
    this->udp_client_cb.rx_thrd_terminate = TRUE;
    }

/*----------------------------------------------------------
Terminate transmit thread if running
----------------------------------------------------------*/
if( this->udp_client_cb.tx_thrd_alive )
    {
    this->udp_client_cb.tx_thrd_terminate = TRUE;
    }

/*----------------------------------------------------------
Close Winsock DLL
----------------------------------------------------------*/
WSACleanup();

}   /* ~UDP_client() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       UDP_client - UDP Client Constructor
*
*   DESCRIPTION:
*       Class constructor for UDP client
*
*********************************************************************/

UDP_client::UDP_client              /* UDP Client Constructor       */
    ( void )                             
{
/*----------------------------------------------------------
Set the socket open status
----------------------------------------------------------*/
this->udp_client_cb.socket_open = FALSE;

}   /* UDP_client() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       UDP_close_socket - Close UDP Socket
*
*   DESCRIPTION:
*       Close UDP socket
*
*********************************************************************/

void UDP_client::UDP_close_socket   /* Close UDP Socket             */
    ( void )
{
/*----------------------------------------------------------
Call the destructor to close the socket
----------------------------------------------------------*/
this->~UDP_client();

}   /* UDP_close_socket() */

/*********************************************************************
*
*   PROCEDURE NAME:
*       UDP_open_socket - Open UDP Socket
*
*   DESCRIPTION:
*       Open UDP socket
*
*********************************************************************/

void UDP_client::UDP_open_socket    /* Open UDP Socket              */
    (
    string              team_name   /* team name                    */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
int						err_no;		/* error number					*/
ostringstream           tmp_str;    /* temporary string             */
UDP_client_q            udp_client_q;
                                    /* udp client queue             */
WSADATA					wsa_data;	/* winsock data					*/

/*----------------------------------------------------------
Initiate Winsock DLL
----------------------------------------------------------*/
WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );

/*----------------------------------------------------------
Attempt to create a socket
----------------------------------------------------------*/
this->udp_skt_fd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

/*----------------------------------------------------------
Verify a valid handle was assigned
----------------------------------------------------------*/
if( this->udp_skt_fd < 0 )
	{
	err_no = WSAGetLastError();

	//UTL ASSERT WITH MSG
	}
 
/*----------------------------------------------------------
Assign UDP server interface information
----------------------------------------------------------*/
this->udp_svr_intfc.sin_family	    = AF_INET;
this->udp_svr_intfc.sin_addr.s_addr = inet_addr( udp_SERVER_IP.c_str() );
this->udp_svr_intfc.sin_port		= htons( udp_SERVER_PORT );

/*----------------------------------------------------------
Initialize the critical section
----------------------------------------------------------*/
InitializeCriticalSection( &this->udp_critical_section );

/*----------------------------------------------------------
Initialize receive queue
----------------------------------------------------------*/
udp_client_q.udp_client_q_init( &this->udp_client_cb.rx_data_q, UDP_BUFFER_SIZE );

/*----------------------------------------------------------
Initialize transmit queue
----------------------------------------------------------*/
udp_client_q.udp_client_q_init( &this->udp_client_cb.tx_data_q, UDP_BUFFER_SIZE );

/*----------------------------------------------------------
Initialize UDP control block
----------------------------------------------------------*/
udp_client_cb.rx_thrd_alive     = FALSE;
udp_client_cb.rx_thrd_terminate = FALSE;
udp_client_cb.tx_thrd_alive     = FALSE;
udp_client_cb.tx_thrd_terminate = FALSE;

/*----------------------------------------------------------
Create the UDP receive thread
----------------------------------------------------------*/
this->udp_client_cb.h_rx_thrd = CreateThread(
                                            NULL,
                                            0,
                                            &UDP_client::udp_receive_thread,
                                            this,
                                            0,
                                            NULL
                                            );

/*----------------------------------------------------------
Verify the receive thread was created successfully
----------------------------------------------------------*/
if( this->udp_client_cb.h_rx_thrd == NULL )
    {
    //UTL ASSERT
    }

/*----------------------------------------------------------
Create the UDP send thread
----------------------------------------------------------*/
this->udp_client_cb.h_tx_thrd = CreateThread(
                                            NULL,
                                            0,
                                            &UDP_client::udp_transmit_thread,
                                            this,
                                            0,
                                            NULL
                                            );

/*----------------------------------------------------------
Verify the send thread was created successfully
----------------------------------------------------------*/
if( this->udp_client_cb.h_tx_thrd == NULL )
    {
    //UTL ASSERT
    }

/*----------------------------------------------------------
Initialize client with team name
----------------------------------------------------------*/
tmp_str << "(init " << team_name << " (version 15.0))";

this->UDP_send( tmp_str.str() );

/*----------------------------------------------------------
Set the socket open status
----------------------------------------------------------*/
this->udp_client_cb.socket_open = TRUE;

}   /* UDP_open_socket() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       UDP_retreive - Retreive UDP Data
*
*   DESCRIPTION:
*       Retreive UDP data from queue if available
*
*********************************************************************/

boolean UDP_client::UDP_retreive	/* UDP Retreive Data            */
	( 
    string * const      rx_data     /* received data                */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
boolean                 ret_val;    /* return value                 */
UDP_client_q            udp_client_q;
                                    /* UDP client queue             */

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
ret_val = false;

/*----------------------------------------------------------
Check if data is available
----------------------------------------------------------*/
if( udp_client_q.udp_client_q_dequeue( &this->udp_client_cb.rx_data_q, rx_data ) )
    {
    ret_val = TRUE;
    }

return( ret_val );

}	/* UDP_retreive() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       UDP_send - Send UDP Data
*
*   DESCRIPTION:
*       Send UDP data when socket is available
*
*********************************************************************/

boolean UDP_client::UDP_send	    /* UDP Send Data                */
	( 
    string              tx_data     /* transmit data                */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
boolean                 ret_val;    /* return value                 */
UDP_client_q            udp_client_q;
                                    /* UDP client queue             */

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
ret_val = false;

/*----------------------------------------------------------
Check if data is available
----------------------------------------------------------*/
if( udp_client_q.udp_client_q_enqueue( &this->udp_client_cb.tx_data_q, (char *)tx_data.c_str() ) )
    {
    ret_val = TRUE;
    }

return( ret_val );

}	/* UDP_send() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_receive - UDP Receive Processing
*
*   DESCRIPTION:
*       Receive packet over UDP socket 
*
*********************************************************************/

void UDP_client::udp_receive	    /* UDP receive processing       */
	( void )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
char                    buf[ udp_SERVER_PKT_SIZE ];
                                    /* temporary buffer             */
int						bytes_read;	/* bytes read					*/
socklen_t				size;		/* size							*/
UDP_client_q            udp_client_q;
                                    /* UDP client queue             */

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
size = sizeof( this->udp_svr_intfc );

/*----------------------------------------------------------
Enter the critical section to ensure this thread processes
with interruption
----------------------------------------------------------*/
EnterCriticalSection( &this->udp_critical_section );

/*----------------------------------------------------------
Read from UDP socket
----------------------------------------------------------*/
bytes_read = recvfrom( 
                     this->udp_skt_fd, 
                     buf, 
                     udp_SERVER_PKT_SIZE, 
                     0, 
                     (sockaddr *)&this->udp_svr_intfc, 
                     &size 
                     );

/*----------------------------------------------------------
Verify the packet was received successfully
----------------------------------------------------------*/
if( bytes_read > 0 )
    {
    udp_client_q.udp_client_q_enqueue( &this->udp_client_cb.rx_data_q, buf );
    }
else
    {
    //UTL ASSERT WITH MSG
    }

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &this->udp_critical_section );

}	/* udp_receive() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_receive_thread - Receive Handling Thread
*
*   DESCRIPTION:
*       Thread to handle UDP receive data
*
*********************************************************************/

DWORD WINAPI UDP_client::udp_receive_thread
                                    /* receive handling thread      */
    (
    LPVOID              lp_param    /* UDP client                   */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
UDP_client *            udp_client_ptr;
                                    /* UDP client pointer           */

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
udp_client_ptr = (UDP_client *)lp_param;

/*----------------------------------------------------------
Indicate thread is alive
----------------------------------------------------------*/
udp_client_ptr->udp_client_cb.rx_thrd_alive = TRUE;

/*----------------------------------------------------------
Loop until thread indicates termination
----------------------------------------------------------*/
while( !udp_client_ptr->udp_client_cb.rx_thrd_terminate )
    {
    udp_client_ptr->udp_receive();

    Sleep( 10 );
    }

/*----------------------------------------------------------
Indicate thread has been terminated
----------------------------------------------------------*/
udp_client_ptr->udp_client_cb.rx_thrd_alive = FALSE;

return( 0 );

}   /* udp_receive_thread() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_transmit - UDP Transmit Processing
*
*   DESCRIPTION:
*       Transmit string over UDP socket
*
*********************************************************************/

void UDP_client::udp_transmit       /* UDP transmit processing		*/
	(
	string		        tx_data		/* transmit data  			    */
	)
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
int					    bytes_sent;	/* bytes sent					*/

/*----------------------------------------------------------
Enter the critical section to ensure this thread processes
with interruption
----------------------------------------------------------*/
EnterCriticalSection( &this->udp_critical_section );

/*----------------------------------------------------------
Send on UDP socket
----------------------------------------------------------*/
bytes_sent = sendto( 
                   this->udp_skt_fd, 
                   tx_data.c_str(), 
                   tx_data.length(), 
                   0, 
                   (sockaddr *)&this->udp_svr_intfc, 
                   (socklen_t)sizeof( this->udp_svr_intfc ) 
                   );

/*----------------------------------------------------------
Verify the packet was sent successfully
----------------------------------------------------------*/
if( bytes_sent < 0) 
	{
	//process_err( "Error: Writing to socket" );
	}

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &this->udp_critical_section );

}	/* udp_transmit() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_transmit_thread - Transmit Handling Thread
*
*   DESCRIPTION:
*       Thread to handle UDP transmit data
*
*********************************************************************/

DWORD WINAPI UDP_client::udp_transmit_thread
                                    /* transmit handling thread     */
    (
    LPVOID              lp_param    /* UDP client                   */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
string                  tmp_str;    /* temporary string             */
UDP_client *            udp_client_ptr;
                                    /* UDP client pointer           */
UDP_client_q            udp_client_q;
                                    /* UDP client queue             */

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
udp_client_ptr = (UDP_client *)lp_param;

/*----------------------------------------------------------
Indicate thread is alive
----------------------------------------------------------*/
udp_client_ptr->udp_client_cb.tx_thrd_alive = TRUE;

/*----------------------------------------------------------
Loop until thread indicates termination
----------------------------------------------------------*/
while( !udp_client_ptr->udp_client_cb.tx_thrd_terminate )
    {
    if( udp_client_q.udp_client_q_dequeue( &udp_client_ptr->udp_client_cb.tx_data_q, &tmp_str ) )
        {
        udp_client_ptr->udp_transmit( tmp_str );
        }

    Sleep( 100 );
    }

/*----------------------------------------------------------
Indicate thread has been terminated
----------------------------------------------------------*/
udp_client_ptr->udp_client_cb.tx_thrd_alive = FALSE;

return( 0 );

}   /* udp_transmit_thread() */