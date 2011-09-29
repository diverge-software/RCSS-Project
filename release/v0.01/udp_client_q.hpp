/*********************************************************************
*
*   MODULE NAME:
*       udp_client_q.hpp - UDP Client Queue Declarations
*
*---------------------------------------------------------------------
* $Id: udp_client_q.hpp, v1.0, 2011-09-23 17:25:00Z, Joseph Wachtel$
* $NoKeywords$
*********************************************************************/

#ifndef _UDP_CLIENT_Q_HPP
#define _UDP_CLIENT_Q_HPP

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <string>

#include "udp_client.hpp"

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

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                               PROCEDURES
--------------------------------------------------------------------*/

boolean udp_client_q_dequeue        /* remove an element from queue */
    (
    udp_client_buf_t * const
                        q_cb,       /* UDP queue control block      */
    string * const      q_data      /* UDP data                     */
    );

boolean udp_client_q_enqueue        /* add an element to queue      */
    (
    udp_client_buf_t * const
                        q_cb,       /* UDP queue control block      */
    char * const        q_data      /* UDP queue data               */
    );

void udp_client_q_init              /* initialize a UDP queue       */
    (
    udp_client_buf_t * const
                        q_cb,       /* UDP queue control block      */
    unsigned int        buf_size    /* UDP buffer size              */
    );

boolean udp_client_q_is_empty       /* return queue empty status    */
    (
    udp_client_buf_t const * const
                        q_cb        /* UDP queue control block      */
    );

#endif /* _UDP_CLIENT_Q_HPP */
