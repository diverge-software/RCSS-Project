/** @file udp_client.cpp
* UDP Client Processing
*
* Handles the threading of both transmit and receive for each client
* which communicates via the UDP protocol with the soccer server
* @author Joseph Wachtel
* @date Oct 19, 2011
*/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <iostream>
#include <sstream>
#include <string>

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


/** Class destructor for UDP client
* @param None
* @pre None
* @post If the UDP socket was opened the socket will be closed.  The
* transmit, receive, and write threads signal termination and the
* debug log is closed.  Finally the Winsock API DLL is closed
*/

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


/** Class constructor for UDP client
* @param None
* @pre None
* @post The UDP control block is initialized to invalid data.  This
* is helpful for determining if a process was successful
*/

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
this->m_client_cb.h_evnt          = NULL;
this->m_client_cb.h_rx_thrd       = NULL;
this->m_client_cb.h_tx_thrd       = NULL;
this->m_client_cb.h_wt_thrd       = NULL;
this->m_client_cb.rx_thrd_alive   = FALSE;
this->m_client_cb.socket          = INVALID_SOCKET;
this->m_client_cb.socket_open     = FALSE;
this->m_client_cb.svr_initialized = FALSE;
this->m_client_cb.stop_rx_thrd    = FALSE;
this->m_client_cb.stop_tx_thrd    = FALSE;
this->m_client_cb.stop_wt_thrd    = FALSE;
this->m_client_cb.tx_thrd_alive   = FALSE;
this->m_client_cb.wt_thrd_alive   = FALSE;

}   /* UDP_client() */


/** Close the UDP socket
* @param None
* @pre None
* @post If the UDP socket was opened the socket will be closed.  The
* transmit, receive, and write threads signal termination and the
* debug log is closed.  Finally the Winsock API DLL is closed
*/

void UDP_client::UDP_close_socket   /* Close UDP Socket             */
    ( void )
{
/*----------------------------------------------------------
Close open UDP socket
----------------------------------------------------------*/
if( this->m_client_cb.socket_open )
    {
    this->m_client_cb.socket_open = FALSE;
    closesocket( this->m_client_cb.socket );
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

}   /* UDP_close_socket() */


/** Disable write debug log to file
* @param None
* @pre None
* @post The write thread is signalled to terminate and the debug log
* is closed
*/

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


/** Enable write debug log to file
* @param filename The filename of the debug log
* @pre The filename given should not already be in use
* @throw If the file cannot be opened, an assertion is thrown
* @throw If the write thread cannot be created, an assertion is thrown
* @post The debug log file is opened and the write thread is 
* started
*/

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


/** Open a UDP socket
* @param server_ip The RoboCup server IP
* @param server_port The RoboCup server port
* @param team_name The Team Name that is to be used
* @param hdl_index Will be removed next sprint
* @pre The UDP control block should be initialized
* @throw If a socket cannot be created, an assertion is thrown
* @throw If the binding of the remote port is unsuccessful, an assertion is thrown
* @throw If the receive thread cannot be created, an assertion is thrown
* @throw If the transmit thread cannot be created, an assertion is thrown
* @post The transmit and receive threads are started and processing begins for each client
* @return Returns the uniform number assigned by the server
*/

int UDP_client::UDP_open_socket     /* Open UDP Socket              */
    (
    string              server_ip,  /* server IP                    */
    unsigned int        server_port,/* server port                  */
    string              team_name,  /* team name                    */
    player_type_t32     player_type /* player type                  */
    )
{
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
int                     ret_val;    /* return value                 */
ostringstream           tmp_str;    /* temporary string             */
WSADATA					wsa_data;	/* winsock data					*/

/*----------------------------------------------------------
Set the team name and role
----------------------------------------------------------*/
m_player.setTeamName( team_name );
m_player.setPlayerRole( player_type );

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
this->m_client_cb.lcl_intfc.sin_port        = htons( 0 );
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
Create the generic event handle
----------------------------------------------------------*/
this->m_client_cb.h_evnt = CreateEvent( NULL, FALSE, FALSE, NULL );

/*----------------------------------------------------------
Verify the handle was created successfully
----------------------------------------------------------*/
if( this->m_client_cb.h_evnt == NULL )
    {
    alwaysAssert();
    }

ResetEvent( this->m_client_cb.h_evnt );

/*----------------------------------------------------------
Initialize client with team name
----------------------------------------------------------*/
tmp_str.str( "" );
tmp_str.clear();

if( player_type != PLAYER_TYPE_TRAINER )
    {
    tmp_str << "(init " << team_name << " (version 15.0))";
    }
else
    {
    tmp_str << "(init (version 15.0))";
    ret_val = 12;
    }

this->UDP_send( tmp_str.str() );

/*----------------------------------------------------------
Set the socket open status
----------------------------------------------------------*/
this->m_client_cb.socket_open = TRUE;

/*----------------------------------------------------------
Verify that a packet was received and parsed from the server
within a specified amount of time
----------------------------------------------------------*/
if( player_type != PLAYER_TYPE_TRAINER )
    {
    if( WaitForSingleObjectEx( this->m_client_cb.h_evnt, SRVR_TIMEOUT, FALSE ) != WAIT_OBJECT_0 )
        {
        this->UDP_close_socket();

        ret_val = -1;
        }
    else
        {
        ret_val = this->m_player.getUniformNumber();
        }
    }

return( ret_val );

}   /* UDP_open_socket() */


/** Send UDP Data
* @param tx_str String To Transmit
* @pre Socket must be open
* @post Queues the string to be sent at the next allowable time
*/
void UDP_client::UDP_send	        /* UDP send data                */
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

}	/* UDP_send() */


/** UDP Completion Routine
* @param err_no Error number from calling function
* @param bytes_xfer Number of bytes transfered
* @param overlapped The overlapped structure which handles asynchronous I/O
* @param flags Flags from calling function
* @pre All Windows CALLBACK's are required to be static
* @throw If the packet cannot be identified, an Assert is thrown
* @post Called when a packet is received.  Parses received UDP packet and passes it to the Decision Processing
*/

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
string                  tx_str;     /* transmit string              */
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
if( udp_client_ptr->m_client_cb.socket_open )
    {
    if( udp_client_ptr->m_player.parseBuffer( udp_client_ptr->m_client_cb.buffer ) )
	    {
        /*--------------------------------------------------
        Enter the critical section to ensure threads will 
        not attempt to concurrently access the same data 
        --------------------------------------------------*/
        EnterCriticalSection( &udp_client_ptr->m_client_cb.rx_crit_sec );

        /*--------------------------------------------------
        Determine if the server has been initialized. This
        indicates that a packet was received and parsed
        successfully. An event is fired to for timeout
        purposes
        --------------------------------------------------*/
        if( !udp_client_ptr->m_client_cb.svr_initialized )
            {
            udp_client_ptr->m_client_cb.svr_initialized = TRUE;

            if( !SetEvent( udp_client_ptr->m_client_cb.h_evnt ) )
                {
                alwaysAssert();
                }
            }

        /*--------------------------------------------------
        Send the parsed data to the debug log string stream
        to be written when the write thread is available.  
        The string stream has low overhead, and is 
        considerably faster to write than writing to a file.  
        This ensures that a packet will not be missed from 
        the slow access time of a hard disk
        --------------------------------------------------*/
        udp_client_ptr->m_client_cb.dbg_log_ss << "##################################" << endl;
        udp_client_ptr->m_client_cb.dbg_log_ss << "##################################" << endl;
        udp_client_ptr->m_client_cb.dbg_log_ss << "########### MESSAGE ##############" << endl;
        udp_client_ptr->m_client_cb.dbg_log_ss << "##################################" << endl;
        udp_client_ptr->m_client_cb.dbg_log_ss << "##################################" << endl;
        udp_client_ptr->m_client_cb.dbg_log_ss << "Message: " << udp_client_ptr->m_client_cb.buffer << endl;

        //udp_client_ptr->m_player.printNewestVisualHash( udp_client_ptr->m_client_cb.dbg_log_ss );
	    //this->m_player.printNewestVisiblePlayersList( this->udp_client_cb.dbg_log );
        //this->m_player.printNewestAuralStruct( this->udp_client_cb.dbg_log );
        //this->m_player.printNewestSenseBodyStruct( this->udp_client_cb.dbg_log );
        //this->m_player.printServerHash( this->udp_client_cb.dbg_log );
        //this->m_player.printPlayerTypesHash( this->udp_client_cb.dbg_log );
        //this->m_player.printPlayerParamHash( this->udp_client_cb.dbg_log );

        /*--------------------------------------------------
        Todo: When the Decision_Processing() is implemented, 
        this will be modified to return a queue.  The queue 
        will be overwritten each time, so the server will 
        only send the most recent commands.  This will 
        ensure the command being sent is based on the most
        recent data
        --------------------------------------------------*/
        //char* demoBuffer = udp_client_ptr->m_client_cb.buffer;
	    //tx_str = makeThemMove( udp_client_ptr->m_client_cb.hdl_idx, demoBuffer );
        Decision_Processing();

	    if( !tx_str.empty() )
	        {
            udp_client_ptr->UDP_send( tx_str );
	        }

        /*--------------------------------------------------
        Leave the critical section
        --------------------------------------------------*/
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
    }

/*----------------------------------------------------------
Clear the asynchronous I/O buffer to ensure no stall data 
is processed
----------------------------------------------------------*/
memset( &udp_client_ptr->m_client_cb.buffer, 0x00, sizeof( udp_client_ptr->m_client_cb.buffer ) );

}   /* udp_completion_routine() */


/** UDP Receive Processing
* @param None
* @pre Socket should be open and initialized
* @throw If WSARecvFrom() returns an unexpected error, and assertion is thrown
* @post Thread sleeps until a packet is received using the overlapped structure.  Once received
* the completion routine is executed
*/

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

    if( this->m_client_cb.socket_open )
        {
        if( WSAGetLastError() != WSA_IO_PENDING )
            {
            process_wsa_err();
            alwaysAssert();
            }
        }
    }

/*----------------------------------------------------------
Leave the critical section
----------------------------------------------------------*/
LeaveCriticalSection( &this->m_client_cb.rx_crit_sec );

}	/* udp_receive() */


/** UDP Receive Processing
* @param udp_client Pointer To UDP Client Object
* @pre Required to be static
* @post Calls the non-static function udp_receive() and sleeps until
* a UDP packet is received
*/

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


/** UDP Transmit Processing
* @param None
* @pre Socket must be open
* @throw If not bytes are transmitted, an assertion is thrown
* @post Transfers the queue of strings over the UDP socket
*/

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


/** Transmit Processing Thread
* @param udp_client Pointer To UDP Client Object
* @pre Required to be static
* @post Calls the non-static function udp_transmit() and sleeps until
* a 100ms has elapsed
*/

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


/** UDP Write Processing
* @param None
* @pre The debug log should be open
* @post Writes to the debug lof from the string stream data
*/

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


/** Write Processing Thread
* @param udp_client Pointer To UDP Client Object
* @pre Required to be static
* @post Calls the non-static function udp_write() and sleeps until
* a 50ms has elapsed
*/

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


/** Process WSA Error
* @param None
* @pre None
* @post Outputs to standard out the Winsock API error message
*/

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