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

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <string.h>
#include <cstdlib>
#include <cmath>
#include <deque>
#include <queue>

#include "Vector2f.hpp"
#include "Parser.hpp"

using namespace Parser;
using namespace std;
using std::tr1::unordered_map;

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
    typedef enum
        {
        PLAYER_TYPE_GOALIE,
        PLAYER_TYPE_FORWARD,
        PLAYER_TYPE_MIDFIELDER,
        PLAYER_TYPE_DEFENDER,
        PLAYER_TYPE_TRAINER
        } player_type_t32;

	#define RIGHT_LINE_X		52.5f
	#define LEFT_LINE_X			-52.5f
	#define RIGHT_BOUNDARY_X	57.6f
	#define LEFT_BOUNDARY_X		-57.6f
	#define TOP_LINE_Y			33.8f		// y-axis is inverted
	#define BOTTOM_LINE_Y		-33.8f		// y-axis is inverted
	#define TOP_BOUNDARY_Y		39.0f		// y-axis is inverted
	#define BOTTOM_BOUNDARY_Y	-39.0f		// y-axis is inverted
	#define PENALTY_RIGHT		36.0f
	#define PENALTY_LEFT		-36.0f
	#define PENALTY_TOP			20.0f		// y-axis inverted
	#define PENALTY_BOTTOM		-20.0f		// y-axis inverted
	#define GOALPOST_TOP_Y		7.0f		// y-axis inverted
	#define GOALPOST_BOTTOM_Y	-7.0f		// y-axis inverted

    void Decision_Processing( void );

    string Catch_Cmd( double direction );
    string Change_View_Cmd( change_view_width_t32 width, change_view_quality_t32 quality );
    string Dash_Cmd( double power );
    string Kick_Cmd( double power, double direction );
    string Move_Cmd( double x_val, double y_val );
    string Say_Cmd( string msg_str );
    string Score_Cmd( void );
    string Sense_Body_Cmd( void );
    string Turn_Cmd( double direction );
    string Turn_Neck_Cmd( double direction ); 

	/** Determines for client-player if a particular teammate can be passed to or if an opponent is in range to intercept
	 * @param teammate A struct containing all visible information about teammate in question
	 * @param opponnents A vector containing structs of an opponents visual information
	 * @pre None
	 * @post Client-Player knows who it can or cannot pass to a particular player
	 * @return True if teammate is passable (open), False if teammate is not
	 */
	bool isTeammateOpenForPass(VisiblePlayer teammate, vector<VisiblePlayer> opponents);

	/** Determines future position of a player given its current position and velocity 
     * @param cPos
     * @param cVec
	 * @pre Non
	 * @post Client-Player knows future of position a player
	 * @return returns future position as a vector
	 */
	Vector2f getFuturePlayerPos(Vector2f cPos, Vector2f cVec, double tInterval, double playerDecay);

	/* Returns the absolute angle the player's body is at, i.e. which direction he would dash in.
	 * Takes into account the angle which the head is turned.
	 * @param absFacingAngle The absolute angle at which the player is facing. In sense body data struct.
	 * @param headAngle Angle the head is turned to. Also in sense body data.
	 * @return Absolute angle the body faces.
	 */
	double getAbsoluteBodyAngle( double absFacingAngle, double headAngle );
	
	/** Determines future position of a player given its current position and velocity 
     * @param cPos Current position of ball.
     * @param cVec Current velocity of ball.
	 * @param tInterval Number of cycles in the future.
	 * @param ballDecay Global server value, how much the ball slows down per cycle.
	 * @pre Non
	 * @post Client-Player knows future of position a player
	 * @return returns future position as a vector
	 */
	Vector2f getFutureBallPos( const Vector2f & cPos, const Vector2f & cVec, double tInterval, double ballDecay);

	/** Returns the data on the player closest to the given position. This position could be that of the
	 * ball, for example, but this function works generally on any object's location.
	 * @param teammates List of visible teammates.
	 * @param opponents List of visible opponents.
	 * @param location The vector location under consideration.
	 * @pre None.
	 * @post None.
	 * @return VisualData on player closest to the given location. Returns a structure with a negative
	 * uniform number on any error.
	 */
	VisiblePlayer getPlayerClosestToLocation( const vector<VisiblePlayer> & teammates, const vector<VisiblePlayer> opponents,
											  const Vector2f & location );

	/** Returns the absolute angle between a player and a location on the field.
	 * @param playerPos Position of the player we're calculating the absolute angle from.
	 * @param location Position that we want to find the absolute angle to.
	 * @pre None.
	 * @post The angle will be between 180 and -180 degrees, and will be in normal Cartesian fashion, rather
	 * than the weird way that the soccer server does it.
	 * @return Absolute angle between the player and the given location.
	 */
	double getAbsAngleToLocation( const Vector2f & playerPos, const Vector2f & location );

	/** Checks to see if the player is within defined boundaries based on role on the field.
	 * @param playerRole the position of the player on the field
	 * @param absLocation the absolute location of the player on the field
	 * @param side the side from which the team is playing
	 * @pre playerRole, absLocation, and side must be initialized.
	 * @post player will be within established boundaries for that player type.
	 */
	bool checkPlayerBounds(player_type_t32 playerRole, Vector2f absLocation, char side);

	/** Determines the side of the opponent. For use with finding scoring goal, etc.
	 * @param side character representing the side that your team is playing from
	 * @pre player initialized
	 * @post None.
	 * @return character representing the other team's side.
	 */
	char getOpponentSide(char side);

	/** Determines if client is in possession of the ball by check the distance of the ball
	 *		the client will stay in possession until someone else has possession
	 * @param distance Distance of the ball from the player  
	 * @pre None.
	 * @post None.
	 * @returns whether or not client possesses the ball 
	 */
	bool doesClientPossessBall( const Vector2f & playerPos, const Vector2f & ballPos );

	/** Determines if the goalie needs to be active, i.e. positioning himself, intercepting,
	 * etc. If not, he should just be chilling back at the goal.
	 * @param side Which side the goalie is playing on.
	 * @param ballPos The absolute position of the ball.
	 * @pre None.
	 * @post None.
	 * @return True if goaile should be active, false otherwise
	 */
	bool goalieShouldBeActive( const char side, const Vector2f & ballPos );

	/** Determines the command that the goalie needs to execute to take possession of the ball
	 * from the enemy team based on its location. If within the penalty box it should catch,
	 * if not the goalie should try to kick if close enough to the ball.
	 * @param side Which side the goalie is playing on.
	 * @param goaliePos The absolute position of the goalie.
	 * @param ballData Visual data of the ball.
	 * @pre Ball must be within sight, and within 2.0m, the minimum catchable distance. This command
	 * MUST be for a goalie since you can get a catch command out of it, an error condition for any
	 * other player role.
	 * @post None.
	 * @return Command to send to the server: catch, kick, or potentially a turn (to face the ball).
	 */
	string goalieDoCatchOrKick( const char side, const Vector2f & goaliePos, const VisualData & ballData );

	/** Use this to turn towards a point, then dash to it in the next cycle.
	 * @param currentPos Player's current position.
	 * @param targetPos Where the player wants to be.
	 * @param absFacingAngle The absolute angle at which the player's head faces.
	 * @param headAngle The angle of the player's head relative to its body.
	 * @param angleToBall Relative angle to ball.
	 * @param dashAfterTurnMode True if the player should dash after the turn command returned, false otherwise.
	 * @param commandQueue Queue of commands, modified to include necessary turn, dash, turn head, etc.
	 * @pre None.
	 * @post dashAfterTurnMode will be set to true if a dash is needed in the next cycle to complete moving
	 * to the target position.
	 */
	void turnThenDash( const Vector2f & currentPos, const Vector2f & targetPos,
					   double absFacingAngle, double headAngle, double angleToBall,
					   bool & dashAfterTurnMode, queue<string> & commandQueue );

	/** Use this to backpedal toward a target point. Much like turnThenDash(), but he dashes back instead.
	 * @param currentPos Player's current position.
	 * @param targetPos Where the player wants to be.
	 * @param absFacingAngle The absolute angle at which the player's head faces.
	 * @param headAngle The angle of the player's head relative to its body.
	 * @param dashAfterTurnMode True if the player should dash after the turn command returned, false otherwise.
	 * @param commandQueue Queue of commands, modified to include necessary turn, dash, turn head, etc.
	 * @pre None.
	 * @post dashAfterTurnMode will be set to true if a dash is needed in the next cycle to complete moving
	 * to the target position.
	 */
	void reverseTurnThenDash( const Vector2f & currentPos, const Vector2f & targetPos,
							  double absFacingAngle, double headAngle,
							  bool & dashAfterTurnMode, queue<string> & commandQueue );

	/* Calculate the position the goalie should be in based on where the ball is, and how far away from the goal
	 * he should be.
	 * @param side The side which he should be on, either 'l' or 'r'.
	 * @param goalPos Absolute position of the goal the goalie is tending.
	 * @param ballPos Absolute position of the ball.
	 * @param radius How far away from the goal the goalie should be standing.
	 * @pre Assumes that side matches the goal that the goalie is tending. E.g. 'l' corresponds to the position of "g l".
	 * @return Position that the goalie should move toward.
	 */
	Vector2f getGoalieTargetPosition( char side, const Vector2f & goalPos, Vector2f ballPos, double radius );
	
	/** Used to determine if the ball is interceptable, can be modified to return point of interceptino
	 * @param ballLoc the location of the ball
	 * @param ballVel the velocity of the ball 
	 * @param clientLoc the location of the client 
	 * @param clientVel the velocity of the client
	 * @param player_speed_max the maximum velocity magnitude of a player. Default max = 1
	 * @param player_accel_max the maximum accelation magnitude of a player. Default max = 1
	 * @param ball_speed_max the maximum speed of the ball. Default maximum = 2.7
	 * @pre None
	 * @post None
	 * @return returns true if the ball is interceptable and false if it is not.
	 */
	bool isBallInterceptable( Vector2f ballLoc, Vector2f ballVel, Vector2f clientLoc, Vector2f clientVel, 
										 double player_speed_max, double player_accel_max, double ball_speed_max, double ball_decay );
	
	/** Determines if a teammate is closer to the ball than this->client
	 * @param teammates a list of teammates
	 * @param ballDistance the distance from this->client to the ball
	 * @return returns true if a teammate is close and false if this->client is closest 
	 */ 
	bool isTeammateCloserBall( vector<VisiblePlayer> teammates, VisualData ballData );
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
/* bounds[x][y]:
 *   [x]: [0] goalie;  [1] forward; [2] midfielder; [3] defender
 *   [y]: [0] right x; [1] left x;  [2] top y;      [3] bottom y
 *
 * another dimension can be added for different modes of play.
 * This is the default. These bounds should work before kick off and during play.
 *
 * NOTE: These bounds have been defined as if playing from the left side of the field.
 *       If you are playing on the right side of the field, this should be corrected
 *       when evaluating the boundary ranges below.
 */
const float bounds[4][4] = {{PENALTY_LEFT,  LEFT_LINE_X,  PENALTY_TOP, PENALTY_BOTTOM},		// Goalie
					        {RIGHT_LINE_X,  -10.0f,		  TOP_LINE_Y,  BOTTOM_LINE_Y},		// Foward/striker
					        {30.0f,	  	    -20.0f,		  TOP_LINE_Y,  BOTTOM_LINE_Y},		// Midfielder
					        {10.0f,		    LEFT_LINE_X,  TOP_LINE_Y,  BOTTOM_LINE_Y}};		// Defender
/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                               PROCEDURES
--------------------------------------------------------------------*/	

#endif /* _AI_PROCESSING_HPP */
