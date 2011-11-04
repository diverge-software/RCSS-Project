/** @file Parser.hpp
* Module used for parsing S-expressions from the server.
* @author Keeler Russell, Jared Mar, Corbin Charpentier
* @date Oct 15, 2011
*/
#ifndef PARSER_HPP
#define PARSER_HPP

/* Contributions by Jared Mar, Keeler Russell, and Corbin Charpentier, all grouped into one big module */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <string.h>
#include <cstdlib>
#include <cmath>

#include "Vector2f.hpp"

using std::string;
using std::vector;
using std::tr1::unordered_map;

namespace Parser
{
	#define PI						3.14159265
	#define INVALID_FLOAT_VALUE		-50000.0
	#define INVALID_UNIFORM_NUMBER	-1
	#define INVALID_STRING_VALUE	"INVALID STRING"
	#define INVALID_TEAM_NAME		INVALID_STRING_VALUE
	#define INVALID_SENDER_NAME		INVALID_STRING_VALUE
	#define INVALID_DIRECTION		-50000.0

	/** @class AuralData
	* Holds data parsed from "(hear ...)" messages from the server.
	*/
	struct AuralData
	{
		int timestamp;
		string sender;
		double direction;
		string message;
	};

	/** @class PlayerParamStruct
	* Holds a float; used for each parameter in the "(player_param ...)" message
	* from the server. Struct wrapping float done for consistency with ServerStruct.
	*/
	struct PlayerParamStruct
	{
		double fValue;
	};

	/** @class PlayerTypeStruct
	* Holds a float; used for each parameter in the "(player_type ...)" message
	* from the server. Struct wrapping float done for consistency with ServerStruct.
	*/
	struct PlayerTypeStruct
	{
		double fValue;
	};

	/** @class SenseBodyData
	* Holds information parsed from "(sense_body ...)" messages from the server.
	* Also holds the absolute position and velocity of the player. This must be
	* calculated from the information transmitted by the server.
	*/
	struct SenseBodyData
	{
		int timestamp;

		//flag indicating absolute coords and velocity have not yet been calculated
		bool absCalculated;

		struct viewModeStruct
		{
			string viewQuality, viewWidth;
			/* NOTE: viewQuality should either be "high" or "low"
					 viewWidth should be "narrow", "normal", or "wide" */
		} view_mode;
		
		Vector2f absLocation;
		Vector2f absVelocity;

		double stamina[3],
			  speed [2],
			  head_angle,
			  kick,
			  dash,
			  turn,
			  say,
			  turn_neck,
			  catchCount,			// IMPORTANT NOTE: "catch" is a reserved keyword, so this has been modified to "catchCount"
			  move,
			  change_view;
		         
		struct armStruct
		{
			double movable, expires, target[2], count;
		} arm;
		
		struct focusStruct
		{
			string target;
			double count;
		} focus;
		
		struct tackleStruct
		{
			double expires, count;
		} tackle;
		
		string collision;
		
		struct foulStruct
		{
			double charged;
			string card;
		} foul;	
	};

	/** @class SeverStruct
	* Holds a float and a string; one will be invalid while the other is valid, depending
	* on the parameter of the "(server_param ...)" message that this corresponds to.
	*/
	struct ServerStruct
	{
		string sValue;
		double fValue;
	};

	/** @class VisualData
	* Holds information parsed from "(see ...)" messages from the server, non-player objects
	* in particular. Player object information is stored elsewhere. Also holds the absolute
	* position and velocity of the objects, such as the ball. This must be calculated from the
	* information transmitted by the server.
	*/
	struct VisualData
	{
		int timestamp;

		double distance;
		double direction;
		double distanceChange;
		double directionChange;

		Vector2f absLocation;
		Vector2f absVelocity;
	};

	/** @class VisiblePlayer
	* Holds information parsed from "(see ...)" messages from the server, for player objects
	* in particular. Note that this also stores a VisualData struct with information about its
	* position, velocity, etc.
	*/
	struct VisiblePlayer
	{
		std::string teamName;
		int uniformNumber;
		bool isGoalie;

		// Has data about relative position/speed/facing as well
		VisualData visualData;
		double bodyDirection;
		double headDirection;
	};

	/** Determines whether an S-expression from the server is complete or not.
	* @param buffer The S-expression to check.
	* @pre None.
	* @post None.
	* @return Returns true when expression is complete, false otherwise.
	*/
	bool isBufferComplete( const string buffer );
	/** Parses an aural S-expression, and stores the result in the auralData struct.
	* @param auralString The aural S-expression to check. E.g. "(hear ...)".
	* @param auralData AuralData struct to be filled with the values parsed from the S-expression.
	* @pre auralString should be a complete and valid (hear ...) message.
	* @post The values in auralData will be overwritten with those parsed from the S-expression.
	*/
	void parseAuralPacket( const string auralString, AuralData & auralData );
	/** Parses an init message S-expression to determine the client's uniform number and ide of play.
	* @param initString The init S-expression to check. E.g. "(init ...)".
	* @param uniformNumber The integer to store the parsed uniform number in.
	* @param side The character to store the parsed side of play in.
	* @pre initString should be a complete and valid (init ...) message.
	* @post uniformNumber will equal the server's assigned uniform number for the client. The side
	* variable will be set to 'l' or 'r' for left side and right side, respectively.
	*/
	void parseInitPacket( const string initString, int & uniformNumber, char & side );
	/** Parses a player parameter S-expression, and stores the result in the playerParams hash.
	* @param buffer The player parameter S-expression to check. E.g. "(player_param ...)".
	* @param playerParams The hash table to store the parsed values in.
	* @pre buffer should be a complete and valid (player_param ...) message.
	* @post The hash table will be filled with the values parsed from the S-expression such that
	* the keys are the strings referring to different paramters specified in the S-expression, and
	* the value is the one parsed for that string.
	*/
	void parsePlayerParamPacket( const string buffer, unordered_map<string, PlayerParamStruct> & playerParams );
	/** Parses a player type S-expression, and stores the result in the array of playerTypes hashes.
	* @param buffer The player type S-expression to check. E.g. "(player_type ...)".
	* @param playerTypes An array of playerType hashes to store the parsed results in.
	* @pre buffer should be a complete and valid (player_type ...) message.
	* @post The id parsed from the S-expression will be the index to the playerType hash that will be filled.
	* The keys are the strings referring to different paramters specified in the S-expression, and the value
	* is the one parsed for that string.
	*/
	void parsePlayerTypePacket( const string buffer, unordered_map<string, PlayerTypeStruct> playerTypes[] );
	/** Parses a sense body S-expression, and stores the result in the sense body struct.
	* @param inData The sense body S-expression to check. E.g. "(sense_body ...)".
	* @param sbd A SenseBodyData  to store the parsed results in.
	* @pre buffer should be a complete and valid (sense_body ...) message.
	* @post The values in sbd will be overwritten with those parsed from the S-expression.
	*/
	void parseSenseBodyPacket( const string inData, SenseBodyData & sbd );
	/** Parses a server parameter S-expression, and stores the result in the serverInfo hash.
	* @param buffer The server parameter S-expression to check. E.g. "(server_param ...)".
	* @param playerParams The hash table to store the parsed values in.
	* @pre buffer should be a complete and valid (server_param ...) message.
	* @post The hash table will be filled with the values parsed from the S-expression such that
	* the keys are the strings referring to different paramters specified in the S-expression, and
	* the value is the one parsed for that string.
	*/
	void parseServerPacket( const string buffer, unordered_map<string, ServerStruct> &serverInfo );
	/** Parses a visual S-expression, and stores the results in visualHash and visiblePlayers.
	* @param visualString The visual S-expression to check. E.g. "(see ...)".
	* @param visualHash The hash table to store visible non-player object data in.
	* @param visiblePlayers The vector to store the visible players data in.
	* @pre visualString should be a complete and valid (see ...) message.
	* @post All player data will be pushed sequentially onto the visiblePlayers vector. All other
	* visible objects will be added to the visualHash, such that the keys are the strings referring
	* to different paramters specified in the S-expression, and the value is the one parsed for that
	* string.
	*/
	void parseVisualPacket( const string visualString, unordered_map<string, VisualData> & visualHash,
							vector<VisiblePlayer> & visiblePlayers );

	/** Converts the relative data in visualHash, visiblePlayers, and senseBodyData to absolute coordinates.
	* @param visualHash A hash table of visual data parsed from a (see ...) message.
	* @param visiblePlayers A vector of visible players parsed from a (see ...) message.
	* @param senseBodyData A structure of sense body data parsed from a (sense_body ...) message.
	* @param stationaryFlags A hash table mapping flag identifiers to their absolute positions on the field.
	* @pre All of the parameters must have valid data in them.
	* @post The visualHash, visiblePlayers list, and senseBodyData struct will now have their relative
	* positioning data converted to absolute coordinates, then stored.
	*/
	void convertToAbsoluteCoordsAndVelocity( unordered_map<string, VisualData> &visualHash, 
							vector<VisiblePlayer> &visiblePlayers, SenseBodyData &senseBodyData,
							unordered_map<string, Vector2f> &stationaryFlags);

	/** Sums two angles to find the absolute angle between relative to the x-axis
	 * @param absAngle Angle of client-player relative to the x-axis
	 * @param refAngle Angle of object relative to absAngle
	 * @pre None
	 * @post None
	 * @return Returns double containing absolute angle of object
	 */
	double getAbsoluteAngleSum(double absAngle, double refAngle);

	/** Fills vector with visible teammates.
	 * @param teamName Name of the allied team.
	 * @param visiblePlayers Vector of VisiblePlayers to look in.
	 * @pre None.
	 * @post None.
	 * @return Returns a vector that contains teammates.
	 */
	vector<VisiblePlayer> getTeammateIdentities( string teamName, const vector<VisiblePlayer> & visiblePlayers );

	/** Fills vector with visible opponents.
	 * @param teamName Name of the allied team.
	 * @param visiblePlayers Vector of VisiblePlayers to look in.
	 * @pre None.
	 * @post None.
	 * @return Returns a vector that contains opponents.
	 */
	vector<VisiblePlayer> getOpponentIdentities( string teamName, const vector<VisiblePlayer> & visiblePlayers );

	/** Fills vector with visible, but unidentified, players.
	 * @param teamName Name of the allied team.
	 * @param visiblePlayers Vector of VisiblePlayers to look in.
	 * @pre None.
	 * @post None.
	 * @return Returns a vector that contains unidentified players.
	 */
	vector<VisiblePlayer> getUnidentifiedIdentities( string teamName, const vector<VisiblePlayer> & visiblePlayers );

	/** If the client is in motion, calculates it's angle relative to the x-axis
	 * @param visualHash hash table that holds all visual data
     * @pre None
	 * @post None
	 * @return Returns the absolute angle relatve to the x-axis
	 */
	double calculateAbsAngle(unordered_map<string, VisualData> &visualHash, string flags[], 
						     unordered_map<string, Vector2f> &stationaryFlags);
}

#endif
