/*********************************************************************
*
*   MODULE NAME:
*       udp_client.cpp - UDP Client Processing
*
*   DESCRIPTION:
*       Performs UDP interface processing
*
*---------------------------------------------------------------------
* $Id: udp_client.cpp, v1.2, 2011-09-23 17:25:00Z, Joseph Wachtel$
* $NoKeywords$
*********************************************************************/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <iostream>
#include <sstream>
#include <string>

#include "debug.hpp"
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
Terminate all running threads
----------------------------------------------------------*/
if( ( this->udp_client_cb.rx_thrd_alive ) 
 || ( this->udp_client_cb.tx_thrd_alive )
 || ( this->udp_client_cb.mn_thrd_alive ) )
    {
    this->udp_client_cb.terminate_thrd = TRUE;
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
    string              server_ip,  /* server IP                    */
    unsigned int        server_port,/* server port                  */
    string              team_name,  /* team name                    */
    unsigned int        hdl_idx     /* handle index                 */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
int						err_no;		/* error number					*/
ostringstream           err_str;    /* error string                 */
char                    filename[ 50 ];
                                    /* filename                     */
ostringstream           tmp_str;    /* temporary string             */
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
if( this->udp_skt_fd == INVALID_SOCKET )
	{
	err_no = WSAGetLastError();

    err_str << "Error: Windows socket() returned error number: " << err_no;
    fatalError( err_str.str() );
	}
 
/*----------------------------------------------------------
Assign UDP server interface information
----------------------------------------------------------*/
this->udp_svr_intfc.sin_family	    = AF_INET;
this->udp_svr_intfc.sin_addr.s_addr = inet_addr( server_ip.c_str() );
this->udp_svr_intfc.sin_port		= htons( server_port );

/*----------------------------------------------------------
Initialize the critical section
----------------------------------------------------------*/
InitializeCriticalSection( &this->udp_critical_section );

/*----------------------------------------------------------
Initialize receive queue
----------------------------------------------------------*/
udp_client_q_init( &this->udp_client_cb.rx_data_q, UDP_BUFFER_SIZE );

/*----------------------------------------------------------
Initialize transmit queue
----------------------------------------------------------*/
udp_client_q_init( &this->udp_client_cb.tx_data_q, UDP_BUFFER_SIZE );

/*----------------------------------------------------------
Initialize UDP control block
----------------------------------------------------------*/
this->udp_client_cb.mn_thrd_alive  = FALSE;
this->udp_client_cb.rx_thrd_alive  = FALSE;
this->udp_client_cb.tx_thrd_alive  = FALSE;
this->udp_client_cb.terminate_thrd = FALSE;

/*----------------------------------------------------------
Set debug log filename
----------------------------------------------------------*/
sprintf_s( filename, sizeof( filename ), "dbg_log_%d.txt", hdl_idx );
this->udp_client_cb.dbg_log.open( filename );

/*----------------------------------------------------------
Create the UDP main thread
----------------------------------------------------------*/
this->udp_client_cb.h_mn_thrd = CreateThread(
                                            NULL,
                                            0,
                                            &UDP_client::udp_main_thread,
                                            this,
                                            0,
                                            NULL
                                            );

/*----------------------------------------------------------
Verify the main thread was created successfully
----------------------------------------------------------*/
if( this->udp_client_cb.h_mn_thrd == NULL )
    {
    alwaysAssert();
    }

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
    alwaysAssert();
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
    alwaysAssert();
    }

/*----------------------------------------------------------
Initialize client with team name
----------------------------------------------------------*/
tmp_str.clear();
tmp_str << "(init " << team_name << " (version 15.0))";

this->UDP_send( tmp_str.str() );

/*----------------------------------------------------------
Set the handle index
----------------------------------------------------------*/
this->udp_client_cb.hdl_idx = hdl_idx;

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

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
ret_val = false;

/*----------------------------------------------------------
Check if data is available
----------------------------------------------------------*/
if( udp_client_q_dequeue( &this->udp_client_cb.rx_data_q, rx_data ) )
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

/*----------------------------------------------------------
Add outgoing data to the TX queue
----------------------------------------------------------*/
ret_val = udp_client_q_enqueue( &this->udp_client_cb.tx_data_q, (char *)tx_data.c_str() );

/*----------------------------------------------------------
An error occurred while trying to add the item to the queue
----------------------------------------------------------*/
fatalAssert( ret_val );

return( ret_val );

}	/* UDP_send() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_main - UDP Main Processing
*
*   DESCRIPTION:
*       UDP main processing 
*
*********************************************************************/

void UDP_client::udp_main	        /* UDP main processing          */
	( void )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
string                  rx_data;    /* received data                */

/*----------------------------------------------------------
Enter the critical section to ensure threads will not
attempt to concurrently access the same data 
----------------------------------------------------------*/
EnterCriticalSection( &this->udp_critical_section );

/*----------------------------------------------------------
Main Processing For Individual Thread

Note: Parsing and all the decision need to happen in this
      thread.  Each client will have a unique thread to
      handle it's own processing.
----------------------------------------------------------*/
if( udp_client_q_dequeue( &this->udp_client_cb.rx_data_q, &rx_data ) )
    {
    /*------------------------------------------------------
    Attempt to parse the UDP message
    ------------------------------------------------------*/
	if( this->m_player.parseBuffer( rx_data ) )
		{
        /*------------------------------------------------------
        Print the parsed data to the debug log
        ------------------------------------------------------*/
	    this->udp_client_cb.dbg_log << "##################################" << endl;
	    this->udp_client_cb.dbg_log << "##################################" << endl;
	    this->udp_client_cb.dbg_log << "########### MESSAGE ##############" << endl;
	    this->udp_client_cb.dbg_log << "##################################" << endl;
	    this->udp_client_cb.dbg_log << "##################################" << endl;
	    this->udp_client_cb.dbg_log << "Message: " << rx_data << endl;
	    this->m_player.printVisualHash( this->udp_client_cb.dbg_log );
	    this->m_player.printServerHash( this->udp_client_cb.dbg_log );
	    this->m_player.printPlayerTypesHash( this->udp_client_cb.dbg_log );
	    this->m_player.printVisiblePlayersList( this->udp_client_cb.dbg_log );
	    this->m_player.printAuralStruct( this->udp_client_cb.dbg_log );
	    this->m_player.printSenseBodyStruct( this->udp_client_cb.dbg_log );
	    this->m_player.printPlayerParamHash( this->udp_client_cb.dbg_log );
		}

    /*------------------------------------------------------
    Parsing of the received message failed
    ------------------------------------------------------*/
	else
		{
		cout << "Message: " << rx_data << endl;
		alwaysAssert();
		}
    }

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &this->udp_critical_section );

}   /* udp_main() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_main_thread - Main Processing Thread
*
*   DESCRIPTION:
*       Thread to handle main processing
*
*********************************************************************/

DWORD WINAPI UDP_client::udp_main_thread
                                    /* main processing thread       */
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
udp_client_ptr->udp_client_cb.mn_thrd_alive = TRUE;

/*----------------------------------------------------------
Loop until thread indicates termination
----------------------------------------------------------*/
while( !udp_client_ptr->udp_client_cb.terminate_thrd )
    {
    udp_client_ptr->udp_main();   

    Sleep( 10 );
    }

/*----------------------------------------------------------
Indicate thread has been terminated
----------------------------------------------------------*/
udp_client_ptr->udp_client_cb.mn_thrd_alive = FALSE;

return( 0 );

}   /* udp_main_thread() */


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

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
size = sizeof( this->udp_svr_intfc );

/*----------------------------------------------------------
Enter the critical section to ensure threads will not
attempt to concurrently access the same data 
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
if( bytes_read != SOCKET_ERROR )
    {
    /*------------------------------------------------------
    Verify there is not a queue overflow
    ------------------------------------------------------*/
    if( !udp_client_q_enqueue( &this->udp_client_cb.rx_data_q, buf ) )
        {
        alwaysAssert();
        }
    }
else
    {
    alwaysAssert();
    }

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &this->udp_critical_section );

}	/* udp_receive() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_receive_thread - Receive Processing Thread
*
*   DESCRIPTION:
*       Thread to handle UDP receive data
*
*********************************************************************/

DWORD WINAPI UDP_client::udp_receive_thread
                                    /* receive processing thread    */
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
while( !udp_client_ptr->udp_client_cb.terminate_thrd )
    {
    Sleep( 10 );

    udp_client_ptr->udp_receive();   
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
Enter the critical section to ensure threads will not
attempt to concurrently access the same data 
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
Verify a socket error did not occur
----------------------------------------------------------*/
if( bytes_sent == SOCKET_ERROR ) 
	{
    alwaysAssert();
	}

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &this->udp_critical_section );

}	/* udp_transmit() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_transmit_thread - Transmit Processing Thread
*
*   DESCRIPTION:
*       Thread to handle UDP transmit data
*
*********************************************************************/

DWORD WINAPI UDP_client::udp_transmit_thread
                                    /* transmit processing thread   */
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
while( !udp_client_ptr->udp_client_cb.terminate_thrd )
    {
    if( udp_client_q_dequeue( &udp_client_ptr->udp_client_cb.tx_data_q, &tmp_str ) )
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