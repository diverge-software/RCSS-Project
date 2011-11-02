/** @file udp_client.hpp
* UDP Client Processing Declarations
*
* Declarations for the UDP Client class
* @author Joseph Wachtel
* @date Oct 19, 2011
*/

#ifndef _UDP_CLIENT_HPP
#define _UDP_CLIENT_HPP

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ws2tcpip.h>
#include <queue>

#include "demo.hpp"
#include "Player.hpp"

using namespace std;

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define UDP_SRVR_PKT_SIZE ( 8192 )  /* UDP server packet size       */
#define SRVR_TIMEOUT      ( 2000 )  /* server timeout in ms         */

/*--------------------------------------------------------------------
                                 TYPES
--------------------------------------------------------------------*/

/** @class udp_client_cb_t
* A container for all the information related to a single UDP client
* including each of the required transmit, receive, and write threads
*/
typedef struct                      /* UDP control block type       */
    {  
    char                buffer[ UDP_SRVR_PKT_SIZE ];
                                    /* asynchronous I/O buffer      */
    ofstream            dbg_log;    /* debug log output file stream */
    boolean             dbg_log_enbl;
                                    /* debug logging enabled        */
    ostringstream       dbg_log_ss; /* debug log string stream      */
    HANDLE              h_evnt;     /* generic event                */
    HANDLE              h_rx_thrd;  /* receive thread handle        */
    HANDLE              h_tx_thrd;  /* transmit thread handle       */
    HANDLE              h_wt_thrd;  /* write thread handle          */  
    sockaddr_in         lcl_intfc;  /* local socket interface       */
    WSAOVERLAPPED       overlapped; /* overlapped structure         */
    CRITICAL_SECTION    rx_crit_sec;/* receive critical section     */
    boolean             rx_thrd_alive; 
                                    /* receive thread alive         */
    SOCKET              socket;     /* UDP socket                   */
    boolean             socket_open;/* socket open                  */
    boolean             stop_tx_thrd;
                                    /* stop transmit thread         */
    boolean             stop_rx_thrd;
                                    /* stop receive thread          */
    boolean             stop_wt_thrd;
                                    /* stop write thread            */
    sockaddr_in         svr_intfc;  /* server socket interface      */
    boolean             svr_initialized;
                                    /* server initialized           */
    CRITICAL_SECTION    tx_crit_sec;/* transmit critical section    */
    queue<string>       tx_data_q;  /* transmit data queue          */
    boolean             tx_thrd_alive; 
                                    /* transmit thread alive        */
    boolean             wt_thrd_alive;
                                    /* write thread alive           */
    } udp_client_cb_t;

/*--------------------------------------------------------------------
                                CLASSES
--------------------------------------------------------------------*/

/** @class UDP_client
* A class which performs the UDP client processing for each of the 
* players
*/
class UDP_client
    {
    public:
        ~UDP_client( void );
        UDP_client( void );

        void UDP_close_socket( void );
        void UDP_dbg_log_dsbl( void );
        void UDP_dbg_log_enbl( string filename );
        boolean UDP_open_socket( string server_ip, unsigned int server_port, string team_name );

    private:
        static void CALLBACK udp_completion_routine( DWORD err_no, DWORD bytes_xfer, LPWSAOVERLAPPED overlapped, DWORD flags );
        static DWORD WINAPI udp_receive_thread( LPVOID udp_client );
        static DWORD WINAPI udp_transmit_thread( LPVOID udp_client );
        static DWORD WINAPI udp_write_thread( LPVOID udp_client );

        void udp_send( string tx_str );
        void udp_receive( void );
        void udp_transmit();
        void udp_write( void );

    protected:
        Player                  m_player;
        udp_client_cb_t         m_client_cb;
     
        static const unsigned   udp_SERVER_PKT_SIZE;    
    };

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

#endif /* _UDP_CLIENT_HPP */
