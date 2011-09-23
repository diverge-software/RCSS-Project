/*********************************************************************
*
*   MODULE NAME:
*       udp_client.hpp - UDP Client Processing Declarations
*
*---------------------------------------------------------------------
* $Id: udp_client.hpp, v1.0, 2011-09-23 17:25:00Z, Joseph Wachtel$
* $NoKeywords$
*********************************************************************/

#ifndef _UDP_CLIENT_HPP
#define _UDP_CLIENT_HPP

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <string>
#include <ws2tcpip.h>

using namespace std;

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define UDP_BUFFER_SIZE ( 10 )      /* UDP buffer size              */

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
    boolean             socket_open;/* socket open                  */
    HANDLE              h_rx_thrd;  /* receive thread handle        */
    HANDLE              h_tx_thrd;  /* transmit thread handle       */

    udp_client_buf_t    rx_data_q;  /* receive data queue           */
    udp_client_buf_t    tx_data_q;  /* transmit data queue          */

    boolean             rx_thrd_alive; 
                                    /* receive thread alive         */
    boolean             rx_thrd_terminate;
                                    /* receive terminate thread     */
    boolean             tx_thrd_alive; 
                                    /* transmit thread alive        */
    boolean             tx_thrd_terminate;
                                    /* transmit terminate thread    */
    } udp_client_cb_t;

class UDP_client                    /* UDP Client Class             */
    {
    public:
        ~UDP_client
            ( void );

        UDP_client
            ( void );

        void UDP_close_socket
            ( void );

        void UDP_open_socket
            (
            string              team_name
            );

        boolean UDP_retreive	
	        ( 
            string * const      rx_data
            );

        boolean UDP_send
	        ( 
            string              tx_data
            );

    private:
        void udp_receive
	        ( void );

       static DWORD WINAPI udp_receive_thread
            (
            LPVOID              lp_param
            );

        void udp_transmit
	        (
	        string		        tx_data
	        );

        static DWORD WINAPI udp_transmit_thread
            (
            LPVOID              lp_param
            );

        udp_client_cb_t         udp_client_cb;
        CRITICAL_SECTION        udp_critical_section;
        SOCKET			        udp_skt_fd;		
        sockaddr_in		        udp_svr_intfc;	

        static const string     udp_SERVER_IP;
        static const unsigned   udp_SERVER_PORT;
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
