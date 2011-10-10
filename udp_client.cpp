/*********************************************************************
*
*   MODULE NAME:
*       udp_client.cpp - UDP Client Processing
*
*   DESCRIPTION:
*       Performs UDP interface processing
*
*---------------------------------------------------------------------
* $Id: udp_client.cpp, v1.3, 2011-10-07 17:25:00Z, Joseph Wachtel$
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

#pragma comment(lib, "ws2_32.lib")

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

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

static void process_wsa_err         /* process WSA error            */
    ( void );


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
Local Constants
----------------------------------------------------------*/
#define MAX_FILENAME_SIZE ( 50 )    /* maximum filename size        */
#define WSA_VER_H         ( 2  )    /* winsock version number high  */
#define WSA_VER_L         ( 2  )    /* winsock version number low   */

/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
int						err_no;		/* error number					*/
ostringstream           err_str;    /* error string                 */
char                    filename[ MAX_FILENAME_SIZE ];
                                    /* filename                     */
ostringstream           tmp_str;    /* temporary string             */
WSADATA					wsa_data;	/* winsock data					*/

/*----------------------------------------------------------
Initiate Winsock DLL
----------------------------------------------------------*/
WSAStartup( MAKEWORD( WSA_VER_H, WSA_VER_L ), &wsa_data );

/*----------------------------------------------------------
Initialize Overlapped structure
----------------------------------------------------------*/
SecureZeroMemory( (PVOID)&this->udp_client_cb.overlapped, sizeof( WSAOVERLAPPED ) );

/*----------------------------------------------------------
Attempt to create a socket
----------------------------------------------------------*/
this->udp_skt_fd = WSASocket( AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED );

/*----------------------------------------------------------
Verify a valid handle was assigned
----------------------------------------------------------*/
if( this->udp_skt_fd == INVALID_SOCKET )
	{
    process_wsa_err();
    alwaysAssert();
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
memset( &this->udp_client_cb.buffer, 0x00, sizeof( this->udp_client_cb.buffer ) );
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
Create the main event

Note: This allows the main thread to sleep until this
      event is set.  This maximizes efficiency and ensures
      processing only occurs when needed
----------------------------------------------------------*/
this->udp_client_cb.h_mn_evnt = CreateEvent( NULL, false, false, NULL );

/*----------------------------------------------------------
Verify the main event was created successfully
----------------------------------------------------------*/
if( this->udp_client_cb.h_mn_evnt == NULL )
    {
    alwaysAssert();
    }

/*----------------------------------------------------------
Assign local address interface information

Note: This is required to use the bind() function that
      allows the use of the asynchronous overlapped I/O
      structure.  The port must be unique
----------------------------------------------------------*/
this->udp_lcl_intfc.sin_family      = AF_INET;
this->udp_lcl_intfc.sin_port        = htons( server_port + hdl_idx + 100 );
this->udp_lcl_intfc.sin_addr.s_addr = htonl( INADDR_ANY );

/*----------------------------------------------------------
Bind the remote address to the local address and port
----------------------------------------------------------*/
err_no = bind( this->udp_skt_fd, (sockaddr *)&this->udp_lcl_intfc, sizeof( this->udp_lcl_intfc ) );

/*----------------------------------------------------------
Verify the bind was successful
----------------------------------------------------------*/
if( err_no == SOCKET_ERROR ) 
    {
    process_wsa_err();
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

this->udp_send( tmp_str.str() );

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
*       udp_completion_routine - UDP Completion Routine
*
*   DESCRIPTION:
*       Completion routine that is called once a complete UDP packet
*       has been received
*
*   Note:
*       All Windows CALLBACK's are required to be static
*
*********************************************************************/

void CALLBACK UDP_client::udp_completion_routine
                                    /* UDP completion routine       */
    (
    DWORD               err_no,     /* error number                 */ 
    DWORD               bytes_xfer, /* bytes transfered             */
    LPWSAOVERLAPPED     overlapped, /* overlapped structure         */ 
    DWORD               flags       /* flags                        */   
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
UDP_client *            udp_client_ptr;
                                    /* UDP client pointer           */

/*----------------------------------------------------------
Assign the void pointer which contains the class "this"
pointer to the local pointer
----------------------------------------------------------*/
udp_client_ptr = (UDP_client *)overlapped->Pointer;

/*----------------------------------------------------------
Enter the critical section to ensure threads will not
attempt to concurrently access the same data 
----------------------------------------------------------*/
EnterCriticalSection( &udp_client_ptr->udp_critical_section );

/*----------------------------------------------------------
Attempt to add the received packet to the queue
----------------------------------------------------------*/
if( !udp_client_q_enqueue( &udp_client_ptr->udp_client_cb.rx_data_q, udp_client_ptr->udp_client_cb.buffer ) )
    {
    alwaysAssert();
    }

/*----------------------------------------------------------
Set the main event to wake the main thread from it's 
suspended state
----------------------------------------------------------*/
SetEvent( udp_client_ptr->udp_client_cb.h_mn_evnt );

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &udp_client_ptr->udp_critical_section );

}   /* udp_completion_routine() */


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

    if( udp_client_q_is_empty( &this->udp_client_cb.tx_data_q ) )
        {
        udp_send( "(move 0 0)" );
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
*   Note:
*       This function, called by CreateThread(), is required to be
*       static.  This function calls a non-static function which is
*       specific to each client
*
*********************************************************************/

DWORD WINAPI UDP_client::udp_main_thread
                                    /* main processing thread       */
    (
    LPVOID              udp_client  /* UDP client                   */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
UDP_client *            udp_client_ptr;
                                    /* UDP client pointer           */

/*----------------------------------------------------------
Assign the passed UDP client to the local pointer.  This
is required due to the requirement of the static function
----------------------------------------------------------*/
udp_client_ptr = (UDP_client *)udp_client;

/*----------------------------------------------------------
Indicate thread is alive
----------------------------------------------------------*/
udp_client_ptr->udp_client_cb.mn_thrd_alive = TRUE;

/*----------------------------------------------------------
Loop until thread indicates termination
----------------------------------------------------------*/
while( !udp_client_ptr->udp_client_cb.terminate_thrd )
    {
    /*------------------------------------------------------
    WaitForSingleObjectEx suspends the thread until 
    the main event occurs.  By using this method, no CPU
    cycles are wasted as the event is only set when a packet
    had been received and processed
    ------------------------------------------------------*/
    WaitForSingleObjectEx( udp_client_ptr->udp_client_cb.h_mn_evnt, INFINITE, TRUE );
    udp_client_ptr->udp_main();
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
int                     err_no;     /* error number                 */
DWORD                   flags;      /* function flags               */
int     				size;		/* size							*/
WSABUF                  tmp_buf;    /* temporary buffer             */

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
flags = false;
size  = sizeof( this->udp_svr_intfc );

/*----------------------------------------------------------
Enter the critical section to ensure threads will not
attempt to concurrently access the same data 
----------------------------------------------------------*/
EnterCriticalSection( &this->udp_critical_section );

/*----------------------------------------------------------
Assign asynchronous I/O buffer to temporary Winsock API
buffer which is compatible with the WSARecvFrom() function

Note: This buffer must be preserved through thread cycles
----------------------------------------------------------*/
tmp_buf.buf = this->udp_client_cb.buffer;
tmp_buf.len = UDP_SRVR_PKT_SIZE;

/*----------------------------------------------------------
Assign the class "this" pointer to the available pointer
in the overlapped structure.  Since the completion routine
for the asynchronous I/O is required to be static, this
allows the class pointer to be used inside the routine
----------------------------------------------------------*/
this->udp_client_cb.overlapped.Pointer = this;

/*----------------------------------------------------------
Receive from the asynchronous I/O socket.  Once the socket
had received a full packet, only then will it call the
completion routine, otherwise the thread is put to sleep.
----------------------------------------------------------*/
err_no = WSARecvFrom( 
                    this->udp_skt_fd,
                    &tmp_buf,
                    1,
                    NULL,
                    &flags,
                    (sockaddr *)&this->udp_svr_intfc,
                    &size, 
                    &this->udp_client_cb.overlapped, 
                    &UDP_client::udp_completion_routine 
                    );

/*----------------------------------------------------------
Check if the asynchronous I/O operation has completed or
if another error has occured
----------------------------------------------------------*/
if( err_no == SOCKET_ERROR )
    {
    err_no = WSAGetLastError();

    if( WSAGetLastError() != WSA_IO_PENDING )
        {
        process_wsa_err();
        alwaysAssert();
        }
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
*   Note:
*       This function, called by CreateThread(), is required to be
*       static.  This function calls a non-static function which is
*       specific to each client
*
*********************************************************************/

DWORD WINAPI UDP_client::udp_receive_thread
                                    /* receive processing thread    */
    (
    LPVOID              udp_client  /* UDP client                   */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
UDP_client *            udp_client_ptr;
                                    /* UDP client pointer           */

/*----------------------------------------------------------
Assign the passed UDP client to the local pointer.  This
is required due to the requirement of the static function
----------------------------------------------------------*/
udp_client_ptr = (UDP_client *)udp_client;

/*----------------------------------------------------------
Indicate thread is alive
----------------------------------------------------------*/
udp_client_ptr->udp_client_cb.rx_thrd_alive = TRUE;

/*----------------------------------------------------------
Loop until thread indicates termination
----------------------------------------------------------*/
while( !udp_client_ptr->udp_client_cb.terminate_thrd )
    {
    /*------------------------------------------------------
    SleepEx will put the thread to sleep and will wake up if
    the time has expired, or an asynchronous I/O operation
    has been completed
    ------------------------------------------------------*/
    SleepEx( 1000, TRUE );

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
*       UDP_send - Send UDP Data
*
*   DESCRIPTION:
*       Send UDP data when socket is available
*
*********************************************************************/

boolean UDP_client::udp_send	    /* UDP send data                */
	( 
    string              tx_str      /* transmit string              */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
boolean                 ret_val;    /* return value                 */

/*----------------------------------------------------------
Add outgoing data to the TX queue
----------------------------------------------------------*/
ret_val = udp_client_q_enqueue( &this->udp_client_cb.tx_data_q, (char *)tx_str.c_str() );

/*----------------------------------------------------------
An error occurred while trying to add the item to the queue
----------------------------------------------------------*/
fatalAssert( ret_val );

return( ret_val );

}	/* udp_send() */


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

Note: Length must be + 1 because the NULL terminated
      character must be included in the server message
----------------------------------------------------------*/
bytes_sent = sendto( 
                   this->udp_skt_fd, 
                   tx_data.c_str(), 
                   tx_data.length() + 1,
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
*   Note:
*       This function, called by CreateThread(), is required to be
*       static.  This function calls a non-static function which is
*       specific to each client
*
*********************************************************************/

DWORD WINAPI UDP_client::udp_transmit_thread
                                    /* transmit processing thread   */
    (
    LPVOID              udp_client  /* UDP client                   */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
string                  tmp_str;    /* temporary string             */
UDP_client *            udp_client_ptr;
                                    /* UDP client pointer           */

/*----------------------------------------------------------
Assign the passed UDP client to the local pointer.  This
is required due to the requirement of the static function
----------------------------------------------------------*/
udp_client_ptr = (UDP_client *)udp_client;

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


/*********************************************************************
*
*   PROCEDURE NAME:
*       process_wsa_err - Process WSA Error
*
*   DESCRIPTION:
*       Process Winsock API error
*
*********************************************************************/

static void process_wsa_err         /* process WSA error            */
    ( void )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
int                     err_code;   /* error code                   */
LPVOID                  msg_buf;    /* message buffer               */

/*----------------------------------------------------------
Get last Winsock API error code
----------------------------------------------------------*/
err_code = WSAGetLastError();

/*----------------------------------------------------------
Attempt to format the error code into a message string
----------------------------------------------------------*/
if( FormatMessage(
                 FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                 FORMAT_MESSAGE_FROM_SYSTEM |
                 FORMAT_MESSAGE_IGNORE_INSERTS,
                 NULL,
                 err_code,
                 NULL,
                 (LPTSTR)&msg_buf,
                 0, 
                 NULL 
                 ) == 0 )
    {
    /*------------------------------------------------------
    An error occured while trying to format the message
    ------------------------------------------------------*/
    LocalFree( msg_buf );
    alwaysAssert();
    }

/*----------------------------------------------------------
Display the error message
----------------------------------------------------------*/
cout << "WSA Error " << err_code << ": " << (LPTSTR)msg_buf << endl;

/*----------------------------------------------------------
Free the local buffer
----------------------------------------------------------*/
LocalFree( msg_buf );

}   /* process_wsa_err() */