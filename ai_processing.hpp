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
