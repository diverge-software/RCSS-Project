/** @file ai_processing.cpp
* Artificial Intelligence Processing
*
* The Artificial Intelligence Processing is the primary module that
* will handle the decision processing and server string formatting for
* each of the clients
* @author Joseph Wachtel
* @date Oct 19, 2011
*/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <iostream>
#include <sstream>
#include <string>

#include "ai_processing.hpp"
#include "debug.hpp"

using namespace std;
using namespace AI_Processing;

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
#define MAX_Y_COORDINATE ( 34    )  /* maximum Y coordinate         */

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

/** Descision Processing
* @param None
* @pre None
* @post Currently under development
*/

void AI_Processing::Decision_Processing      
                                    /* Descision Processing         */
    ( void )
{

}   /* Decision_Processing() */


/** Catch Command
* @param direction Directional value to catch given in degrees
* @pre None
* @post None
* @throw If the value of direction is out of range, an Assertion is thrown
* @return Returns the formatted Catch command string for the server
*/

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


/** Change View Command
* @param width An enumeration value which represents the width of the view
* @param quality An enumeration value which represents the quality of the view
* @pre None
* @post None
* @throw If the value of width or quality is invalid, an Assertion is thrown
* @return Returns the formatted Change View command string for the server
*/

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


/** Dash Command
* @param power Power value which is unitless
* @pre None
* @post None
* @throw If the value of power is out of range, an Assertion is thrown
* @return Returns the formatted Dash command string for the server
*/

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


/** Kick Command
* @param power Power value which is unitless
* @pre None
* @post None
* @throw If the value of power is out of range, an Assertion is thrown
* @return Returns the formatted Kick command string for the server
*/

string AI_Processing::Kick_Cmd      /* Kick Command                 */
    (
    double              power,       /* power value                  */
	double              direction   /* direction value              */
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

if( ( direction < MIN_MOMENT )
 || ( direction > MAX_MOMENT ) )
{
	alwaysAssert();
}


ret_val << "(kick " << power << " " << direction << ")";

return( ret_val.str() );

}   /* Kick_Cmd() */


/** Move Command
* @param x_val X coordinate value which is unitless
* @param y_val Y coordinate value which is unitless
* @pre None
* @post None
* @throw If the value of x_val or y_val is out of range, an Assertion is thrown
* @return Returns the formatted Move command string for the server
*/

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


/** Say Command
* @param msg_str Message that is to be said
* @pre None
* @post None
* @return Returns the formatted Say command string for the server
*/

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


/** Score Command
* @param None
* @pre None
* @post None
* @return Returns the formatted Score command string for the server
*/

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


/** Sense Body Command
* @param None
* @pre None
* @post None
* @return Returns the formatted Sense Body command string for the server
*/

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


/** Turn Command
* @param direction Directional value to turn given in degrees
* @pre None
* @post None
* @throw If the value of direction is out of range, an Assertion is thrown
* @return Returns the formatted Turn command string for the server
*/

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


/** Turn Neck Command
* @param direction Directional value to turn neck given in degrees
* @pre None
* @post None
* @throw If the value of direction is out of range, an Assertion is thrown
* @return Returns the formatted Turn Neck command string for the server
*/

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

bool AI_Processing::isTeammateOpenForPass(VisiblePlayer teammate, vector<VisiblePlayer> opponents)
{
	//determine if an opponent is in position to intercept
	for(unsigned int i=0; i < opponents.size(); i++)
	{
		if( (opponents[i].visualData.distance <= teammate.visualData.distance)
			 && (opponents[i].visualData.direction <= 5)
			 && (opponents[i].visualData.direction >= -5) )
		{		
			return false;
		}
	}

	return true;	
}

Vector2f AI_Processing::getFuturePlayerPos(Vector2f cPos, Vector2f cVec, double tInterval, double playerDecay)
{
	return cPos + cVec * pow( playerDecay, tInterval );
}

Vector2f AI_Processing::getFutureBallPos( const Vector2f & cPos, const Vector2f & cVec, double tInterval, double ballDecay)
{
	return cPos + cVec * pow(ballDecay, tInterval-1); 
}

VisiblePlayer AI_Processing::getPlayerClosestToLocation( const vector<VisiblePlayer> & teammates,
														 const vector<VisiblePlayer> opponents,
														 const Vector2f & location )
{
	VisiblePlayer closestPlayer;
	closestPlayer.uniformNumber = -1;

	if( !teammates.empty() )
	{
		closestPlayer = teammates[0];
		for( unsigned int i = 1; i < teammates.size(); i++ )
		{
			if( ( teammates[i].visualData.absLocation - location ).magnitude()
				< ( closestPlayer.visualData.absLocation - location ).magnitude() )
			{
				closestPlayer = teammates[i];
			}
		}
	}
	if( !opponents.empty() )
	{
		int startIndex = 0;
		if( closestPlayer.uniformNumber == -1 )
		{
			closestPlayer = opponents[0];
			startIndex = 1;
		}
		for( unsigned int i = startIndex; i < opponents.size(); i++ )
		{
			if( ( opponents[i].visualData.absLocation - location ).magnitude()
				< ( closestPlayer.visualData.absLocation - location ).magnitude() )
			{
				closestPlayer = opponents[i];
			}
		}
	}

	return closestPlayer;
}

double AI_Processing::getAbsAngleToLocation( const Vector2f & playerPos, const Vector2f & location )
{
	double xDifference = location[0] - playerPos[0];
	double yDifference = location[1] - playerPos[1];

	return atan2( yDifference, xDifference ) * ( 180 / PI );
}

bool AI_Processing::checkPlayerBounds(player_type_t32 playerRole, Vector2f absLocation, char side)
{
	/************ NEEDS TO BE TESTED ************************************
	 * This function should only be used if the player has moved since the last cycle
	 * should help with efficiency.
     ********************************************************************/

	// Check if the player's position is within the defined boundaries
	if( side == 'l' &&									// evaluate if on the left side
		absLocation[0] < bounds[playerRole][0] &&
		absLocation[0] > bounds[playerRole][1] &&
		absLocation[1] < bounds[playerRole][2] &&
		absLocation[1] > bounds[playerRole][3] )
	{
		return true;
	}
	else if ( side == 'r' &&								// evaluate if on the right side
		absLocation[0] < -1 * bounds[playerRole][1] &&	// swap the x bounds and multiply by -1.
		absLocation[0] > -1 * bounds[playerRole][0] &&
		absLocation[1] < bounds[playerRole][2] &&		// y values should be the same
		absLocation[1] > bounds[playerRole][3] )
	{
		return true;
	}
	else
	{
		// What the hell? GET BACK IN YOUR CAGE.
		return false;

		// resetPlayerPosition();
		
		// or maybe have something like
		// if the ball is far away
		// and the player is within 1 meter of a boundary,
		// just stop and rest
	}
}

char AI_Processing::getOpponentSide(char side)
{
	if(side == 'r')
	{
		return ( 'l' );
	}
	else if (side == 'l')
	{
		return ( 'r' );
	}
	else
	{
		alwaysAssert();
	}
}

bool AI_Processing::doesClientPossessBall( const Vector2f & playerPos, const Vector2f & ballPos )
{
	double distance = ( playerPos - ballPos ).magnitude();

	return ( distance < 1 && distance > -1 );
}

bool AI_Processing::goalieShouldBeActive( const char side, const Vector2f & ballPos )
{
	return ( ( side == 'l' && ballPos[0] <= 0 ) ||
			 ( side == 'r' && ballPos[0] >= 0 ) );
}

string AI_Processing::goalieDoCatchOrKick( const char side, const Vector2f & goaliePos, const VisualData & ballData )
{
	string command;

	// If within the penalty box, catch the ball
	if( checkPlayerBounds( PLAYER_TYPE_GOALIE, goaliePos, side ) )
	{
		command = Catch_Cmd( ballData.direction );
	}
	// If not within the penalty box, if the ball is close enough to kick
	else if( ballData.distance < 0.7 )
	{
		// In the future, should pass to teammates, but for now, just kick in some random direction
		command = Kick_Cmd( 100, -20 );
	}
	// Otherwise, we're close enough, just turn toward the ball
	else if( ballData.direction != 0 )
	{
		command = Turn_Cmd( ballData.direction );
	}

	return command;
}

string AI_Processing::turnThenDash( const Vector2f & currentPos, const Vector2f & targetPos, double absFacingAngle, bool & dashAfterTurnMode )
{
	if( !dashAfterTurnMode )
	{
		double turnAngle = absFacingAngle - getAbsAngleToLocation( currentPos, targetPos );

		if( turnAngle < -180 )
		{
			turnAngle += 360;
		}
		else if( turnAngle > 180 )
		{
			turnAngle -= 360;
		}

		// If we can basically see the point, just dash to it
		if( fabs( turnAngle ) < 4 )
		{
			return Dash_Cmd( ( currentPos - targetPos ).magnitude() );
		}
		else
		{
			return Turn_Cmd( turnAngle );
			dashAfterTurnMode = true;
		}
	}
	else
	{
		return Dash_Cmd( ( currentPos - targetPos ).magnitude() );
		dashAfterTurnMode = false;
	}
}