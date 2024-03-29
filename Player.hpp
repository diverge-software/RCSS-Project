/** @file Player.hpp
* Represents a player on the field.
* @author Keeler Russell, Jared Mar, Corbin Charpentier
* @date Oct 13, 2011
*/
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Parser.hpp"
#include "Vector2f.hpp"
#include "ai_processing.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <deque>
#include <queue>

using namespace std;
using namespace Parser;

#define NUM_PLAYER_TYPES	17
#define MAX_QUEUE_SIZE		30

/** @class Player
* A class which represents a player: memory, sensory processing, and thinking.
*/
class Player
{
	public:
        char	side;

		/** Default constructor. Initializes invalid values for aural and sense body queues.
		* Also initializes stationary flag positions.
		* @pre None.
		* @post This object is ready to receive server messages and parse/store their data.
		*/
		Player();
		
		/** Parses a buffer of information from the soccer server.
		* @param buffer Any S-expression string sent from the soccer server.
		* @pre None.
		* @post The relevant data will be stored in the private members of this object based
		* on the type of message parsed. If an error occured, this function will hit an assertion.
		*/
		bool parseBuffer( const string buffer );

		/** Prints the most recently stored visual information hash to the specified output stream.
		* @param os The output stream to write to.
		* @pre None.
		* @post The contents of the hash will be printed to the output stream.
		*/
		void printNewestVisualHash( ostream & os ) const;
		
		/** Prints the most recently stored list of visible players to the specified output stream.
		* @param os The output stream to write to.
		* @pre None.
		* @post The contents of the visible player vector will be printed to the output stream.
		*/
		void printNewestVisiblePlayersList( ostream & os ) const;
		void printNewestVisiblePlayersListHelper( ostream & os, vector<VisiblePlayer> players ) const;
		
		/** Prints the most recently stored aural information struct to the specified output stream.
		* @param os The output stream to write to.
		* @pre None.
		* @post The contents of the aural struct will be printed to the output stream.
		*/
		void printNewestAuralStruct( ostream & os ) const;
		
		/** Prints the most recently stored sense body information struct to the specified output stream.
		* @param os The output stream to write to.
		* @pre None.
		* @post The contents of the sense body struct will be printed to the output stream.
		*/
		void printNewestSenseBodyStruct( ostream & os ) const;
		
		/** Prints the contents of the server information hash to the specified output stream.
		* @param os The output stream to write to.
		* @pre None.
		* @post The contents of the hash will be printed to the output stream.
		*/
		void printServerHash( ostream & os ) const;
		
		/** Prints the contents of the player types hash to the specified output stream.
		* @param os The output stream to write to.
		* @pre None.
		* @post The contents of the hash will be printed to the output stream.
		*/
		void printPlayerTypesHash( ostream & os ) const;
		
		/** Prints the contents of the palyer parameter hash to the specified output stream.
		* @param os The output stream to write to.
		* @pre None.
		* @post The contents of the hash will be printed to the output stream.
		*/
		void printPlayerParamHash( ostream & os ) const;

		/** Retrieves the position of the object from the visual data, estimating its position
		* linearly based on past data if necessary.
		* @param objName Name of the object under consideration. E.g. "b" for ball.
		* @param currentTimestamp Timestamp of latest data, used for linear estimation.
		* @pre objName should be a valid identifier for a game object.
		* @post None.
		* @return Returns the position of the object, and will return a vector with invalid
		* float values if the object cannot be found and its position cannot be estimated.
		*/
		Vector2f getObjectPosition( string objName, int currentTimestamp ) const;
		
		/** Sets team name.
		 * @param teamname name of team
		 * @pre None
		 * @post Private member teamName is set to appropriate team name	
		 */
		void setTeamName(string teamname);
		
		/** Set the player's role, or position, on the field.
		 * @param role Can be either "goalie", "defender", "midfielder", or "forward".
		 * @pre A valid string is passed in.
		 * @post The player's type is set.
		 */
		void setPlayerRole( AI_Processing::player_type_t32 role );
		
		/** Get the player's uniform number.
		 * @return Integer uniform number.
		 */
		int getUniformNumber() const;
		/** Returns whether or not the player has received the (init ...) message.
		 * This is applicable for offline trainers, as well.
		 * @return True if player has received an (init ...) message, false otherwise.
		 */
		bool isPlayerInitialized() const;

		/** Determines what the player should do next, based on his role.
		 * @param None.
		 * @pre The player should be initialized with data from the server.
		 * @post None.
		 * @return Queue of string commands to send to the server for the client's action.
		 */
		void think( queue<string> & commandQueue );

		/** Determines the actions for forwards
		 * @pre Player should be intialized with data from the server.
		 * @post None.
		 * @return command initialized with string to send to server to make the forward do something.
		 */
		void think_forward( AI_Processing::player_type_t32 playerRole, queue<string> & commandQueue );

		/** Retrieves the contents of playMode
		 * @pre Player initialized.
		 * @post None.
		 * @return the contents of playMode
		 */
		string getPlayMode() const;
		
		/** Determines which team possesses the ball
		 * @param commandQueue Holds list of commands to send to server 
		 * @pre None
		 * @post Possession booleans are updated
		 * @return Nothing
		 */
		void determinePossession( queue<string> & commandQueue );

	private:	
		// Sensory data passed from the server
		deque<AuralData>                          mAuralDataQueue;
		deque<SenseBodyData>                      mSenseBodyDataQueue;
		deque<vector<VisiblePlayer>>			  mTeammateListQueue;
		deque<vector<VisiblePlayer>>			  mOpponentListQueue;
		deque<vector<VisiblePlayer>>			  mUnidentifiedPlayerListQueue;
		deque<unordered_map<string, VisualData>>  mVisualDataQueue;

		// Initialization data passed from the server
		int		uniformNumber;
		string	teamName;
		AI_Processing::player_type_t32 playerRole;

		unordered_map<string, PlayerTypeStruct>   mPlayerTypes[NUM_PLAYER_TYPES];
		unordered_map<string, ServerStruct>       mServerInfo;
		unordered_map<string, PlayerParamStruct>  mPlayerParams;
		unordered_map<string, Vector2f>           mStationaryFlags;

		// Game states:
		bool	playerInitialized;
		bool	servInitialized;
		bool	kickOffMode;
		bool	playOn;
		bool	freeKickFriendly;
		bool	freeKickOpponent;

		bool	teamPossessesBall;
		bool	clientPossessesBall;
		int		uNumPossessesBall;

		// Used to keep track of if we're trying to turn, then dash to a location.
		// This should be used if the turn has just been sent, and the dash needs
		// to happen
		bool		dashAfterTurnMode;
		bool		goalieInPosition;
		bool		viewModeSet;
		Vector2f	targetPoint;


		// forwards keys
		bool top;


		string  playMode;
};
	
#endif
