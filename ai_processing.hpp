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

	// Used for stationary flags
	// Defines locations of various lines on the field.
	#define RIGHT_LINE_X		50.0f
	#define LEFT_LINE_X			-50.0f
	#define RIGHT_BOUNDARY_X	55.0f
	#define LEFT_BOUNDARY_X		-55.0f
	#define TOP_LINE_Y			40.0f
	#define BOTTOM_LINE_Y		-40.0f
	#define TOP_BOUNDARY_Y		45.0f
	#define BOTTOM_BOUNDARY_Y	-45.0f
	#define PENALTY_RIGHT		( RIGHT_LINE_X - 18.0f )
	#define PENALTY_LEFT		( LEFT_LINE_X + 18.0f )
	#define PENALTY_TOP			22.0f
	#define PENALTY_BOTTOM		-22.0f
	#define GOALPOST_TOP_Y		10.0f
	#define GOALPOST_BOTTOM_Y	-10.0f

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
	Vector2f getFuturePlayerPos(Vector2f cPos, Vector2f cVec, double tInterval);
	
	/** Determines future position of a player given its current position and velocity 
     * @param cPos
     * @param cVec
	 * @pre Non
	 * @post Client-Player knows future of position a player
	 * @return returns future position as a vector
	 */
	Vector2f getFutureBallPos(Vector2f cPos, Vector2f cVec, double tInterval, double ballDecay);   

	/** Checks to see if the player is within defined boundaries based on role on the field.
	 * @param playerRole the position of the player on the field
	 * @param absLocation the absolute location of the player on the field
	 * @param side the side from which the team is playing
	 * @pre playerRole, absLocation, and side must be initialized.
	 * @post player will be within established boundaries for that player type.
	 */
	void checkPlayerBounds(player_type_t32 playerRole, Vector2f absLocation, char side);

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
	bool doesClientPossessBall(double distance);
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
