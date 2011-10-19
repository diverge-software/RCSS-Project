/*********************************************************************
*
*   MODULE NAME:
*       udp_client.cpp - UDP Client Processing
*
*   DESCRIPTION:
*       Performs UDP interface processing
*
*---------------------------------------------------------------------
* $Id: udp_client.cpp, v1.5, 2011-10-11 17:25:00Z, Joseph Wachtel$
* $NoKeywords$
*********************************************************************/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <iostream>
#include <sstream>
#include <string>

#include "ai_processing.hpp"
#include "debug.hpp"
#include "udp_client.hpp"

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
if( this->m_client_cb.socket_open )
    {
    closesocket( this->m_client_cb.socket );
    this->m_client_cb.socket_open = FALSE;
    }

/*----------------------------------------------------------
Terminate all running threads
----------------------------------------------------------*/
if( this->m_client_cb.rx_thrd_alive )
    {
    this->m_client_cb.stop_rx_thrd = TRUE;
    }

if( this->m_client_cb.tx_thrd_alive )
    {
    this->m_client_cb.stop_tx_thrd = TRUE;
    }

if( this->m_client_cb.wt_thrd_alive )
    {
    this->m_client_cb.stop_wt_thrd = TRUE;
    }

/*----------------------------------------------------------
Close debug log file
----------------------------------------------------------*/
if( this->m_client_cb.dbg_log.is_open() )
    {
    this->m_client_cb.dbg_log.close();
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
Initialize UDP control block
----------------------------------------------------------*/
memset( &this->m_client_cb.buffer, 0x00, sizeof( this->m_client_cb.buffer ) );
memset( &this->m_client_cb.lcl_intfc, 0x00, sizeof( this->m_client_cb.lcl_intfc ) );
memset( &this->m_client_cb.svr_intfc, 0x00, sizeof( this->m_client_cb.svr_intfc ) );
this->m_client_cb.dbg_log_ss.clear();
this->m_client_cb.dbg_log_ss.str( "" );
this->m_client_cb.h_rx_thrd     = NULL;
this->m_client_cb.h_tx_thrd     = NULL;
this->m_client_cb.h_wt_thrd     = NULL;
this->m_client_cb.hdl_idx       = 0;
this->m_client_cb.rx_thrd_alive = FALSE;
this->m_client_cb.socket        = INVALID_SOCKET;
this->m_client_cb.socket_open   = FALSE;
this->m_client_cb.stop_rx_thrd  = FALSE;
this->m_client_cb.stop_tx_thrd  = FALSE;
this->m_client_cb.stop_wt_thrd  = FALSE;
this->m_client_cb.tx_thrd_alive = FALSE;
this->m_client_cb.wt_thrd_alive = FALSE;

}   /* UDP_client() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       UDP_dbg_log_dsbl - Disable Write Debug Log To File
*
*   DESCRIPTION:
*       Disable write debug log to file
*
*********************************************************************/

void UDP_client::UDP_dbg_log_dsbl   /* Dsbl Write Debug Log To File */
    ( void )
{
/*----------------------------------------------------------
Stop the write thread if it is running
----------------------------------------------------------*/
if( this->m_client_cb.wt_thrd_alive )
    {
    this->m_client_cb.stop_wt_thrd = TRUE;
    }

/*----------------------------------------------------------
Close debug log file
----------------------------------------------------------*/
if( this->m_client_cb.dbg_log.is_open() )
    {
    this->m_client_cb.dbg_log.close();
    }

}   /* UDP_dbg_log_dsbl() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       UDP_dbg_log_enbl - Enable Write Debug Log To File
*
*   DESCRIPTION:
*       Enable write debug log to file
*
*********************************************************************/

void UDP_client::UDP_dbg_log_enbl   /* Enbl Write Debug Log To File */
    (
    string              filename    /* filename of debug log        */
    )
{
/*----------------------------------------------------------
Attempt to open the specified file
----------------------------------------------------------*/
if( !this->m_client_cb.dbg_log.is_open() )
    {
    this->m_client_cb.dbg_log.open( filename.c_str() );

    /*------------------------------------------------------
    Verify the file was opened successfully
    ------------------------------------------------------*/
    if( this->m_client_cb.dbg_log.fail() )
        {
        alwaysAssert();
        }
    }

/*----------------------------------------------------------
Create the UDP write thread
----------------------------------------------------------*/
if( this->m_client_cb.h_wt_thrd == NULL )
    {
    this->m_client_cb.h_wt_thrd = CreateThread(
                                              NULL,
                                              0,
                                              &UDP_client::udp_write_thread,
                                              this,
                                              0,
                                              NULL
                                              );

    /*------------------------------------------------------
    Verify the write thread was created successfully
    ------------------------------------------------------*/
    if( this->m_client_cb.h_wt_thrd == NULL )
        {
        alwaysAssert();
        }

    this->m_client_cb.stop_wt_thrd = FALSE;
    }

}   /* UDP_dbg_log_enbl() */


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
//set teamName
m_player.setTeamName(team_name);
/*----------------------------------------------------------
Local Constants
----------------------------------------------------------*/
#define WSA_VER_H ( 2 )             /* winsock version number high  */
#define WSA_VER_L ( 2 )             /* winsock version number low   */

/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
int						err_no;		/* error number					*/
string                  filename;   /* filename                     */
ostringstream           tmp_str;    /* temporary string             */
WSADATA					wsa_data;	/* winsock data					*/

/*----------------------------------------------------------
Initiate Winsock DLL
----------------------------------------------------------*/
WSAStartup( MAKEWORD( WSA_VER_H, WSA_VER_L ), &wsa_data );

/*----------------------------------------------------------
Initialize Overlapped structure
----------------------------------------------------------*/
SecureZeroMemory( (PVOID)&this->m_client_cb.overlapped, sizeof( WSAOVERLAPPED ) );

/*----------------------------------------------------------
Attempt to create a socket
----------------------------------------------------------*/
this->m_client_cb.socket = WSASocket( 
                                    AF_INET, 
                                    SOCK_DGRAM, 
                                    IPPROTO_UDP, 
                                    NULL, 
                                    0, 
                                    WSA_FLAG_OVERLAPPED 
                                    );

/*----------------------------------------------------------
Verify a valid socket was assigned
----------------------------------------------------------*/
if( this->m_client_cb.socket == INVALID_SOCKET )
	{
    process_wsa_err();
    alwaysAssert();
	}
 
/*----------------------------------------------------------
Assign UDP server interface information
----------------------------------------------------------*/
this->m_client_cb.svr_intfc.sin_family      = AF_INET;
this->m_client_cb.svr_intfc.sin_addr.s_addr = inet_addr( server_ip.c_str() );
this->m_client_cb.svr_intfc.sin_port        = htons( server_port );

/*----------------------------------------------------------
Initialize the critical sections
----------------------------------------------------------*/
InitializeCriticalSection( &this->m_client_cb.rx_crit_sec );
InitializeCriticalSection( &this->m_client_cb.tx_crit_sec );

/*----------------------------------------------------------
Assign local address interface information

Note: This is required to use the bind() function that
      allows the use of the asynchronous overlapped I/O
      structure.  The port must be unique
----------------------------------------------------------*/
this->m_client_cb.lcl_intfc.sin_family      = AF_INET;
this->m_client_cb.lcl_intfc.sin_port        = htons( server_port + hdl_idx + 100 );
this->m_client_cb.lcl_intfc.sin_addr.s_addr = htonl( INADDR_ANY );

/*----------------------------------------------------------
Bind the remote address to the local address and port
----------------------------------------------------------*/
err_no = bind( 
             this->m_client_cb.socket, 
             (sockaddr *)&this->m_client_cb.lcl_intfc, 
             sizeof( this->m_client_cb.lcl_intfc ) 
             );

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
this->m_client_cb.h_rx_thrd = CreateThread(
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
if( this->m_client_cb.h_rx_thrd == NULL )
    {
    alwaysAssert();
    }

/*----------------------------------------------------------
Ensure no packets are dropped by given the receive thread
time critical priority
----------------------------------------------------------*/
SetThreadPriority( this->m_client_cb.h_rx_thrd, THREAD_PRIORITY_TIME_CRITICAL );

/*----------------------------------------------------------
Create the UDP send thread
----------------------------------------------------------*/
this->m_client_cb.h_tx_thrd = CreateThread(
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
if( this->m_client_cb.h_tx_thrd == NULL )
    {
    alwaysAssert();
    }

/*----------------------------------------------------------
Initialize client with team name
----------------------------------------------------------*/
tmp_str.str( "" );
tmp_str.clear();
tmp_str << "(init " << team_name << " (version 15.0))";

this->udp_send( tmp_str.str() );

/*----------------------------------------------------------
Set the handle index
----------------------------------------------------------*/
this->m_client_cb.hdl_idx = hdl_idx;

/*----------------------------------------------------------
Set the socket open status
----------------------------------------------------------*/
this->m_client_cb.socket_open = TRUE;

}   /* UDP_open_socket() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_completion_routine - UDP Completion Routine
*
*   DESCRIPTION:
*       Completion routine that is called once a complete UDP packet
*       has been received.  This routine will also handle the parsing
*       and decision making of the received packet
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
AI_Processing           ai_processing;
UDP_client *            udp_client_ptr;
                                    /* UDP client pointer           */

/*----------------------------------------------------------
Assign the void pointer which contains the class "this"
pointer to the local pointer
----------------------------------------------------------*/
udp_client_ptr = (UDP_client *)overlapped->Pointer;

/*----------------------------------------------------------
Main Processing For Individual Thread

Note: Parsing and all the decision need to happen in this
      thread.  Each client will have a unique thread to
      handle it's own processing.
----------------------------------------------------------*/
if( udp_client_ptr->m_player.parseBuffer( udp_client_ptr->m_client_cb.buffer ) )
	{
    /*------------------------------------------------------
    Enter the critical section to ensure threads will not
    attempt to concurrently access the same data 
    ------------------------------------------------------*/
    EnterCriticalSection( &udp_client_ptr->m_client_cb.rx_crit_sec );

    /*------------------------------------------------------
    Send the parsed data to the debug log string stream
    to be written when the write thread is available.  The
    string stream has low overhead, and is considerably
    faster to write than writing to a file.  This ensures
    that a packet will not be missed from the slow access
    time of a hard disk
    ------------------------------------------------------*/
    udp_client_ptr->m_client_cb.dbg_log_ss << "##################################" << endl;
    udp_client_ptr->m_client_cb.dbg_log_ss << "##################################" << endl;
    udp_client_ptr->m_client_cb.dbg_log_ss << "########### MESSAGE ##############" << endl;
    udp_client_ptr->m_client_cb.dbg_log_ss << "##################################" << endl;
    udp_client_ptr->m_client_cb.dbg_log_ss << "##################################" << endl;
    udp_client_ptr->m_client_cb.dbg_log_ss << "Message: " << udp_client_ptr->m_client_cb.buffer << endl;

    //this->m_player.printNewestVisualHash( this->udp_client_cb.dbg_log );
	//this->m_player.printNewestVisiblePlayersList( this->udp_client_cb.dbg_log );
    //this->m_player.printNewestAuralStruct( this->udp_client_cb.dbg_log );
    //this->m_player.printNewestSenseBodyStruct( this->udp_client_cb.dbg_log );
    //this->m_player.printServerHash( this->udp_client_cb.dbg_log );
    //this->m_player.printPlayerTypesHash( this->udp_client_cb.dbg_log );
    //this->m_player.printPlayerParamHash( this->udp_client_cb.dbg_log );

    /*------------------------------------------------------
    Leave the critical section
    ------------------------------------------------------*/
    LeaveCriticalSection( &udp_client_ptr->m_client_cb.rx_crit_sec );
	}

/*----------------------------------------------------------
Parsing of the received message failed
----------------------------------------------------------*/
else
	{
	cout << "Message: " << udp_client_ptr->m_client_cb.buffer << endl;
	alwaysAssert();
	}

/*----------------------------------------------------------
Clear the asynchronous I/O buffer to ensure no stall data 
is processed
----------------------------------------------------------*/
memset( &udp_client_ptr->m_client_cb.buffer, 0x00, sizeof( udp_client_ptr->m_client_cb.buffer ) );

}   /* udp_completion_routine() */


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
flags = FALSE;
size  = sizeof( this->m_client_cb.svr_intfc );

/*----------------------------------------------------------
Enter the critical section to ensure threads will not
attempt to concurrently access the same data 
----------------------------------------------------------*/
EnterCriticalSection( &this->m_client_cb.rx_crit_sec );

/*----------------------------------------------------------
Assign asynchronous I/O buffer to temporary Winsock API
buffer which is compatible with the WSARecvFrom() function

Note: This buffer must be preserved through thread cycles
----------------------------------------------------------*/
tmp_buf.buf = this->m_client_cb.buffer;
tmp_buf.len = UDP_SRVR_PKT_SIZE;

/*----------------------------------------------------------
Assign the class "this" pointer to the available pointer
in the overlapped structure.  Since the completion routine
for the asynchronous I/O is required to be static, this
allows the class pointer to be used inside the routine
----------------------------------------------------------*/
this->m_client_cb.overlapped.Pointer = this;

/*----------------------------------------------------------
Receive from the asynchronous I/O socket.  Once the socket
had received a full packet, only then will it call the
completion routine, otherwise the thread is put to sleep.
----------------------------------------------------------*/
err_no = WSARecvFrom( 
                    this->m_client_cb.socket,
                    &tmp_buf,
                    1,
                    NULL,
                    &flags,
                    (sockaddr *)&this->m_client_cb.svr_intfc,
                    &size, 
                    &this->m_client_cb.overlapped, 
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
LeaveCriticalSection( &this->m_client_cb.rx_crit_sec );

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
udp_client_ptr->m_client_cb.rx_thrd_alive = TRUE;

/*----------------------------------------------------------
Loop until thread indicates termination
----------------------------------------------------------*/
while( !udp_client_ptr->m_client_cb.stop_rx_thrd )
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
udp_client_ptr->m_client_cb.rx_thrd_alive = FALSE;

return( 0 );

}   /* udp_receive_thread() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_send - Send UDP Data
*
*   DESCRIPTION:
*       Send UDP data when socket is available
*
*********************************************************************/

void UDP_client::udp_send	        /* UDP send data                */
	( 
    string              tx_str      /* transmit string              */
    )
{
/*----------------------------------------------------------
Enter the critical section to ensure threads will not
attempt to concurrently access the same data 
----------------------------------------------------------*/
EnterCriticalSection( &this->m_client_cb.tx_crit_sec );

/*----------------------------------------------------------
Add server command to queue.  This must be a queue to allow
more than one command to be sent in a single cycle which is
allowed for the "turn neck" client control command.  The 
calling function is responsible for determing which commands
are allowed to be combined
----------------------------------------------------------*/
this->m_client_cb.tx_data_q.push( tx_str );

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &this->m_client_cb.tx_crit_sec );

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
	( void )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
int					    bytes_sent;	/* bytes sent					*/
string                  tx_str;     /* transmit string              */

/*----------------------------------------------------------
Enter the critical section to ensure threads will not
attempt to concurrently access the same data 
----------------------------------------------------------*/
EnterCriticalSection( &this->m_client_cb.tx_crit_sec );

/*----------------------------------------------------------
Send on UDP socket until transmit queue has been emptied

Note: Length must be + 1 because the NULL terminated
      character must be included in the server message
----------------------------------------------------------*/
while( !this->m_client_cb.tx_data_q.empty() )
    {
    tx_str = this->m_client_cb.tx_data_q.front();
    this->m_client_cb.tx_data_q.pop();

    bytes_sent = sendto( 
                       this->m_client_cb.socket, 
                       tx_str.c_str(), 
                       tx_str.length() + 1,
                       0, 
                       (sockaddr *)&this->m_client_cb.svr_intfc, 
                       (socklen_t)sizeof( this->m_client_cb.svr_intfc ) 
                       );

    /*----------------------------------------------------------
    Verify a socket error did not occur
    ----------------------------------------------------------*/
    if( bytes_sent == SOCKET_ERROR ) 
	    {
        alwaysAssert();
	    }
    }

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &this->m_client_cb.tx_crit_sec );

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
udp_client_ptr->m_client_cb.tx_thrd_alive = TRUE;

/*----------------------------------------------------------
Loop until thread indicates termination
----------------------------------------------------------*/
while( !udp_client_ptr->m_client_cb.stop_tx_thrd )
    {
    if( !udp_client_ptr->m_client_cb.tx_data_q.empty() )
        {
        udp_client_ptr->udp_transmit();
        }

    Sleep( 100 );
    }

/*----------------------------------------------------------
Indicate thread has been terminated
----------------------------------------------------------*/
udp_client_ptr->m_client_cb.tx_thrd_alive = FALSE;

return( 0 );

}   /* udp_transmit_thread() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_write - UDP Write Processing
*
*   DESCRIPTION:
*       Write to debug log from string stream data
*
*********************************************************************/

void UDP_client::udp_write          /* UDP write processing		    */
	( void )
{
/*----------------------------------------------------------
Enter the critical section to ensure threads will not
attempt to concurrently access the same data 
----------------------------------------------------------*/
EnterCriticalSection( &this->m_client_cb.rx_crit_sec );

/*----------------------------------------------------------
Write to the string stream to the debug log.  Using this
method is much more efficient then writing line by line
with a queue
----------------------------------------------------------*/
this->m_client_cb.dbg_log << this->m_client_cb.dbg_log_ss.str();

/*----------------------------------------------------------
Clear the string stream
----------------------------------------------------------*/
this->m_client_cb.dbg_log_ss.str( "" );
this->m_client_cb.dbg_log_ss.clear();

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &this->m_client_cb.rx_crit_sec );

}	/* udp_write() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_write_thread - Write Processing Thread
*
*   DESCRIPTION:
*       Thread to handle writing the debug log to a file. This 
*       process takes several milliseconds to complete, therefore
*       this has to be a separate thread to prevent missed packets
*
*   Note:
*       This function, called by CreateThread(), is required to be
*       static. This function calls a non-static function which is
*       specific to each client
*
*********************************************************************/

DWORD WINAPI UDP_client::udp_write_thread
                                    /* write processing thread      */
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
udp_client_ptr->m_client_cb.wt_thrd_alive = TRUE;

/*----------------------------------------------------------
Loop until thread indicates termination
----------------------------------------------------------*/
while( !udp_client_ptr->m_client_cb.stop_wt_thrd )
    {
    Sleep( 50 );

    if( udp_client_ptr->m_client_cb.socket_open )
        {
        udp_client_ptr->udp_write();
        }
    }

/*----------------------------------------------------------
Indicate thread has been terminated
----------------------------------------------------------*/
udp_client_ptr->m_client_cb.wt_thrd_alive = FALSE;

return( 0 );

}   /* udp_write_thread() */


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