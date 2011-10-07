/*********************************************************************
*
*   MODULE NAME:
*       udp_client.hpp - UDP Client Processing Declarations
*
*---------------------------------------------------------------------
* $Id: udp_client.hpp, v1.2, 2011-09-23 17:25:00Z, Joseph Wachtel$
* $NoKeywords$
*********************************************************************/

#ifndef _UDP_CLIENT_HPP
#define _UDP_CLIENT_HPP

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <fstream>
#include <string>
#include <ws2tcpip.h>

#include "Player.hpp"

using namespace std;

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define UDP_BUFFER_SIZE ( 100 )     /* UDP buffer size              */

/*--------------------------------------------------------------------
                                 TYPES
--------------------------------------------------------------------*/

typedef struct                      /* UDP client buffer type       */
    {
    string              buffer[ UDP_BUFFER_SIZE ];
                                    /* buffer                       */
    unsigned int        start;      /* start position               */
    unsigned int        stop;       /* stop position                */
    unsigned int        avail;      /* number of available slots    */
    unsigned int        buf_mask;   /* buffer mask                  */
    boolean             overflow;   /* buffer overflow              */
    } udp_client_buf_t;

typedef struct                      /* UDP control block type       */
    {
    ofstream            dbg_log;    /* debug log                    */
    boolean             socket_open;/* socket open                  */

    HANDLE              h_mn_thrd;  /* main thread handle           */
    HANDLE              h_rx_thrd;  /* receive thread handle        */
    HANDLE              h_tx_thrd;  /* transmit thread handle       */
    HANDLE              h_completion_prt;
    HANDLE              h_rd_evnt;
    HANDLE              h_mn_evnt;
    
    char                buffer[ 8192 ];
    
    WSAOVERLAPPED       overlapped;

    unsigned int        hdl_idx;    /* handle index                 */

    udp_client_buf_t    rx_data_q;  /* receive data queue           */
    udp_client_buf_t    tx_data_q;  /* transmit data queue          */

    boolean             terminate_thrd;
                                    /* terminate thread             */

    boolean             mn_thrd_alive;
                                    /* main thread alive            */
    boolean             rx_thrd_alive; 
                                    /* receive thread alive         */
    boolean             tx_thrd_alive; 
                                    /* transmit thread alive        */
    } udp_client_cb_t;

/*--------------------------------------------------------------------
                                CLASSES
--------------------------------------------------------------------*/

class UDP_client
    {
    public:
        ~UDP_client( void );
        UDP_client( void );

        void UDP_close_socket( void );
        void UDP_open_socket( string server_ip, unsigned int server_port,string team_name,unsigned int hdl_idx );

        boolean UDP_retreive ( string * const rx_data );
        boolean UDP_send( string tx_data );

    private:
        static DWORD WINAPI udp_main_thread( LPVOID lp_param );
        static DWORD WINAPI udp_receive_thread( LPVOID lp_param );
        static DWORD WINAPI udp_transmit_thread( LPVOID lp_param );

        void udp_main( void );
        void udp_receive( void );
        void udp_transmit( string tx_data );

        friend boolean udp_client_q_dequeue( udp_client_buf_t * const q_cb, string * const q_data );
        friend boolean udp_client_q_enqueue( udp_client_buf_t * const q_cb, char * const q_data );
        friend void udp_client_q_init( udp_client_buf_t * const q_cb, unsigned int buf_size );
        friend boolean udp_client_q_is_empty( udp_client_buf_t const * const q_cb );

        static void CALLBACK CompletionROUTINE( DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags );

    protected:
        Player                  m_player;
        udp_client_cb_t         udp_client_cb;
        CRITICAL_SECTION        udp_critical_section;
        SOCKET			        udp_skt_fd;		
        sockaddr_in		        udp_svr_intfc;
        sockaddr_in		        local_addr;	
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
