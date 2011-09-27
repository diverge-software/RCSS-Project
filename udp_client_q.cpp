/*********************************************************************
*
*   MODULE NAME:
*       udp_client_q.cpp - UDP Client Queue Processing
*
*   DESCRIPTION:
*       Performs UDP queue processing
*
*---------------------------------------------------------------------
* $Id: udp_client_q.cpp, v1.0, 2011-09-23 17:25:00Z, Joseph Wachtel$
* $NoKeywords$
*********************************************************************/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_client_q_dequeue - Remove An Element From Queue
*
*   DESCRIPTION:
*       Remove the oldest element in UDP queue and copy it to the
*       buffer of the calling function and returns TRUE if successful,
*       otherwise FALSE is returned
*
*   NOTES:
*       Assumes the UDP queue control block pointer and data pointer
*       are valid
*
*********************************************************************/

boolean udp_client_q_dequeue        /* remove an element from queue */
    (
    udp_client_buf_t * const
                        q_cb,       /* UDP queue control block      */
    string * const      q_data      /* UDP data                     */
    )
{
/*----------------------------------------------------------
Local variables
----------------------------------------------------------*/
boolean                 ret_val;    /* return value                 */

/*----------------------------------------------------------
No data in the queue to retrieve
----------------------------------------------------------*/
if( q_cb->avail == ( q_cb->buf_mask + 1 ) )
    {
    /*------------------------------------------------------
    Set return value
    ------------------------------------------------------*/
    ret_val = FALSE;
    }

/*----------------------------------------------------------
Data in queue to retrieve
----------------------------------------------------------*/
else
    {
    /*------------------------------------------------------
    Set return value
    ------------------------------------------------------*/
    ret_val = TRUE;

    /*------------------------------------------------------
    Copy data from UDP queue to data buffer
    ------------------------------------------------------*/
    q_data->assign( q_cb->buffer[ q_cb->start ] );

    /*------------------------------------------------------
    Adjust queue control block data

    Note: Using modulo operator instead of and operator
          because buf_mask is not a factor of 2^n - 1
    ------------------------------------------------------*/
    q_cb->start    = ( q_cb->start + 1 ) % q_cb->buf_mask;
    q_cb->avail    = ( q_cb->avail + 1 );
    q_cb->overflow = FALSE;
    }

return( ret_val );

}   /* udp_client_q_dequeue() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_client_q_enqueue - Add An Element To Queue
*
*   DESCRIPTION:
*       Adds an element to the end of the UDP queue and returns TRUE
*       if successful, otherwise FALSE is returned
*
*   NOTES:
*       Assumes the UDP queue control block pointer and data pointer
*       are valid
*
*********************************************************************/

boolean udp_client_q_enqueue        /* add an element to queue      */
    (
    udp_client_buf_t * const
                        q_cb,       /* UDP queue control block      */
    char * const        q_data      /* UDP queue data               */
    )
{
/*----------------------------------------------------------
Local variables
----------------------------------------------------------*/
boolean                 ret_val;    /* return value                 */

/*----------------------------------------------------------
Queue overflow
----------------------------------------------------------*/
if( q_cb->avail == 0 )
    {
    /*------------------------------------------------------
    Set return value
    ------------------------------------------------------*/
    ret_val = FALSE;

    /*------------------------------------------------------
    Set buffer overflow flag
    ------------------------------------------------------*/
    q_cb->overflow = TRUE;
    }

/*----------------------------------------------------------
Queue slot available
----------------------------------------------------------*/
else
    {
    /*------------------------------------------------------
    Set return value
    ------------------------------------------------------*/
    ret_val = TRUE;

    /*------------------------------------------------------
    Add an element of data to the end of data queue
    ------------------------------------------------------*/
    q_cb->buffer[ q_cb->stop ].assign( q_data );

    /*------------------------------------------------------
    Adjust queue control block data

    Note: Using modulo operator instead of and operator
          because buf_mask is not a factor of 2^n - 1
    ------------------------------------------------------*/
    q_cb->stop  = ( q_cb->stop + 1 ) % q_cb->buf_mask;
    q_cb->avail = ( q_cb->avail - 1 );
    }

return( ret_val );

}   /* udp_client_q_enqueue() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_client_q_init - Initialize a UDP Queue
*
*   DESCRIPTION:
*       Initializes a UDP queue
*
*   NOTES:
*       Assumes the UDP queue control block pointer is valid
*
*********************************************************************/

void udp_client_q_init              /* initialize a UDP queue       */
    (
    udp_client_buf_t * const
                        q_cb,       /* UDP queue control block      */
    unsigned int        buf_size    /* UDP buffer size              */
    )
{
/*----------------------------------------------------------
Initialize a UDP queue
----------------------------------------------------------*/
q_cb->avail    = buf_size;
q_cb->buf_mask = buf_size - 1;
q_cb->start    = 0;
q_cb->stop     = 0;
q_cb->overflow = FALSE;

}   /* udp_client_q_init() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       udp_client_q_is_empty - Return Queue Empty Status
*
*   DESCRIPTION:
*       Returns TRUE if the queue is empty
*
*   NOTES:
*       Assumes the UDP queue control block pointer is valid
*
*********************************************************************/

boolean udp_client_q_is_empty       /* return queue empty status    */
    (
    udp_client_buf_t const * const
                        q_cb        /* UDP queue control block      */
    )
{
/*----------------------------------------------------------
Local variables
----------------------------------------------------------*/
boolean                 ret_val;    /* return value                 */

/*----------------------------------------------------------
Set return value
----------------------------------------------------------*/
ret_val = ( q_cb->avail == ( q_cb->buf_mask + 1 ) );

return( ret_val );

}   /* udp_client_q_is_empty() */