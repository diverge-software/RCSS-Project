/** @file ai_processing.hpp
* Artificial Intelligence Processing Declarations
*
* Declarations for the Artificial Intelligence namespace
* @author Joseph Wachtel
* @date Oct 19, 2011
*/

#ifndef _AI_PROCESSING_HPP
#define _AI_PROCESSING_HPP

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include "udp_client.hpp"

using namespace std;

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                 TYPES
--------------------------------------------------------------------*/

typedef enum                        /* change view width type       */
    {
    CV_WIDTH_NARROW,                /* narrow width value           */
    CV_WIDTH_NORMAL,                /* normal width value           */
    CV_WIDTH_WIDE                   /* wide width value             */
    } change_view_width_t32;

typedef enum                        /* change view quality type     */
    {
    CV_QUALITY_LOW,                 /* low quality value            */
    CV_QUALITY_HIGH                 /* high quality value           */
    } change_view_quality_t32;

/*--------------------------------------------------------------------
                                CLASSES
--------------------------------------------------------------------*/

namespace AI_Processing
    {
    void Decision_Processing( void );

    string Catch_Cmd( double direction );
    string Change_View_Cmd( change_view_width_t32 width, change_view_quality_t32 quality );
    string Dash_Cmd( double power );
    string Kick_Cmd( double power );
    string Move_Cmd( double x_val, double y_val );
    string Say_Cmd( string msg_str );
    string Score_Cmd( void );
    string Sense_Body_Cmd( void );
    string Turn_Cmd( double direction );
    string Turn_Neck_Cmd( double direction );    
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

#endif /* _AI_PROCESSING_HPP */
