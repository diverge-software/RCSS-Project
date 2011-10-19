/*********************************************************************
*
*   MODULE NAME:
*       ai_processing.cpp - Artificial Intelligence Processing
*
*   DESCRIPTION:
*       Performs artificial intelligence processing
*
*---------------------------------------------------------------------
* $Id: ai_processing.cpp, v1.0, 2011-10-11 17:25:00Z, Joseph Wachtel$
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

using namespace std;

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define MIN_MOMENT       ( -180  )  /* minimum moment in degrees    */
#define MAX_MOMENT       ( 180   )  /* maximum moment in degrees    */
#define MIN_POWER        ( -100  )  /* minimum power                */
#define MAX_POWER        ( 100   )  /* maximum power                */
#define MIN_NECK_MOMENT  ( -180  )  /* minimum neck moment in deg   */
#define MAX_NECK_MOMENT  ( 180   )  /* maximum neck moment in deg   */
#define MIN_X_COORDINATE ( -52.5 )  /* minimum X coordinate         */
#define MAX_X_COORDINATE ( 52.5  )  /* maximum X coordinate         */
#define MIN_Y_COORDINATE ( -34   )  /* minimum Y coordinate         */
#define MAX_Y_COORDINATE ( -34   )  /* maximum Y coordinate         */

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
*       Decision_Processing - Descision Processing
*
*   DESCRIPTION:
*       Artificial intelligence decision processing for client
*
*********************************************************************/

void AI_Processing::Decision_Processing      
                                    /* Descision Processing         */
    ( void )
{

}   /* Decision_Processing() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Catch_Cmd - Catch Command
*
*   DESCRIPTION:
*       Format Catch command string for the server
*
*********************************************************************/

string AI_Processing::Catch_Cmd     /* Catch Command                */
    (
    double              direction   /* directional value            */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
ostringstream           ret_val;    /* return value                 */

/*----------------------------------------------------------
Verify the input parameter(s) are within the defined 
server boundaries
----------------------------------------------------------*/
if( ( direction < MIN_MOMENT )
 || ( direction > MAX_MOMENT ) )
    {
    alwaysAssert();
    }

ret_val << "(catch " << direction << ")";

return( ret_val.str() );

}   /* Catch_Cmd() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Change_View_Cmd - Change View Command
*
*   DESCRIPTION:
*       Format Change View command string for the server
*
*********************************************************************/

string AI_Processing::Change_View_Cmd
                                    /* Change View Command          */
    (
    change_view_width_t32
                        width,      /* width value                  */
    change_view_quality_t32
                        quality     /* quality value                */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
string                  quality_str;/* quality string               */
ostringstream           ret_val;    /* return value                 */
string                  width_str;  /* width string                 */

/*----------------------------------------------------------
Convert Width enumeration to string value
----------------------------------------------------------*/
switch( width )
    {
    case CV_WIDTH_NARROW:
        width_str = "narrow";
        break;

    case CV_WIDTH_NORMAL:
        width_str = "normal";
        break;

    case CV_WIDTH_WIDE:
        width_str = "wide";
        break;

    default:
        alwaysAssert();
    }

/*----------------------------------------------------------
Convert Quality enumeration to string value
----------------------------------------------------------*/
switch( quality )
    {
    case CV_QUALITY_LOW:
        quality_str = "low";
        break;

    case CV_QUALITY_HIGH:
        quality_str = "high";
        break;

    default:
        alwaysAssert();
    }

ret_val << "(change_view " << width_str << " " << quality_str << ")";

return( ret_val.str() );

}   /* Change_View_Cmd() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Dash_Cmd - Dash Command
*
*   DESCRIPTION:
*       Format Dash command string for the server
*
*********************************************************************/

string AI_Processing::Dash_Cmd      /* Dash Command                 */
    (
    double              power       /* power value                  */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
ostringstream           ret_val;    /* return value                 */

/*----------------------------------------------------------
Verify the input parameter(s) are within the defined 
server boundaries
----------------------------------------------------------*/
if( ( power < MIN_POWER )
 || ( power > MAX_POWER ) )
    {
    alwaysAssert();
    }

ret_val << "(dash " << power << ")";

return( ret_val.str() );

}   /* Dash_Cmd() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Kick_Cmd - Kick Command
*
*   DESCRIPTION:
*       Format Kick command string for the server
*
*********************************************************************/

string AI_Processing::Kick_Cmd      /* Kick Command                 */
    (
    double              power       /* power value                  */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
ostringstream           ret_val;    /* return value                 */

/*----------------------------------------------------------
Verify the input parameter(s) are within the defined 
server boundaries
----------------------------------------------------------*/
if( ( power < MIN_POWER )
 || ( power > MAX_POWER ) )
    {
    alwaysAssert();
    }

ret_val << "(kick " << power << ")";

return( ret_val.str() );

}   /* Kick_Cmd() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Move_Cmd - Move Command
*
*   DESCRIPTION:
*       Format Move command string for the server
*
*********************************************************************/

string AI_Processing::Move_Cmd      /* Move Command                 */
    (
    double              x_val,      /* X coordinate value           */
    double              y_val       /* Y coordinate value           */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
ostringstream           ret_val;    /* return value                 */

/*----------------------------------------------------------
Verify the input parameter(s) are within the defined 
server boundaries
----------------------------------------------------------*/
if( ( x_val < MIN_X_COORDINATE )
 || ( x_val > MAX_X_COORDINATE )
 || ( y_val < MIN_Y_COORDINATE )
 || ( y_val > MAX_Y_COORDINATE ) )
    {
    alwaysAssert();
    }

ret_val << "(move " << x_val << " " << y_val << ")";

return( ret_val.str() );

}   /* Move_Cmd() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Say_Cmd - Say Command
*
*   DESCRIPTION:
*       Format Say command string for the server
*
*********************************************************************/

string AI_Processing::Say_Cmd       /* Say Command                  */
    (
    string              msg_str     /* message to say               */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
ostringstream           ret_val;    /* return value                 */

ret_val << "(say " << msg_str << ")";

return( ret_val.str() );

}   /* Say_Cmd() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Score_Cmd - Score Command
*
*   DESCRIPTION:
*       Format Score command string for the server
*
*********************************************************************/

string AI_Processing::Score_Cmd     /* Score Command                */
    ( void )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
ostringstream           ret_val;    /* return value                 */

ret_val << "(score)";

return( ret_val.str() );

}   /* Score_Cmd() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Sense_Body_Cmd - Sense Body Command
*
*   DESCRIPTION:
*       Format Sense Body command string for the server
*
*********************************************************************/

string AI_Processing::Sense_Body_Cmd/* Sense Body Command           */
    ( void )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
ostringstream           ret_val;    /* return value                 */

ret_val << "(sense_body)";

return( ret_val.str() );

}   /* Sense_Body_Cmd() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Turn_Cmd - Turn Command
*
*   DESCRIPTION:
*       Format Turn command string for the server
*
*********************************************************************/

string AI_Processing::Turn_Cmd      /* Turn Command                 */
    (
    double              direction   /* directional value            */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
ostringstream           ret_val;    /* return value                 */

/*----------------------------------------------------------
Verify the input parameter(s) are within the defined 
server boundaries
----------------------------------------------------------*/
if( ( direction < MIN_MOMENT )
 || ( direction > MAX_MOMENT ) )
    {
    alwaysAssert();
    }

ret_val << "(turn " << direction << ")";

return( ret_val.str() );

}   /* Turn_Cmd() */


/*********************************************************************
*
*   PROCEDURE NAME:
*       Turn_Neck_Cmd - Turn Neck Command
*
*   DESCRIPTION:
*       Format Turn Neck command string for the server
*
*********************************************************************/

string AI_Processing::Turn_Neck_Cmd /* Turn Neck Command            */
    (
    double              direction   /* directional value            */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
ostringstream           ret_val;    /* return value                 */

/*----------------------------------------------------------
Verify the input parameter(s) are within the defined 
server boundaries
----------------------------------------------------------*/
if( ( direction < MIN_MOMENT )
 || ( direction > MAX_MOMENT ) )
    {
    alwaysAssert();
    }

ret_val << "(turn_neck " << direction << ")";

return( ret_val.str() );

}   /* Turn_Neck_Cmd() */