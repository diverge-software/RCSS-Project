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

#include "Vector2f.hpp"

using std::string;
using std::vector;
using std::tr1::unordered_map;

namespace Parser
{
	#define INVALID_FLOAT_VALUE		-50000.0
	#define INVALID_UNIFORM_NUMBER	-1
	#define INVALID_STRING_VALUE	"INVALID STRING"
	#define INVALID_TEAM_NAME		INVALID_STRING_VALUE
	#define INVALID_SENDER_NAME		INVALID_STRING_VALUE
	#define INVALID_DIRECTION		-50000.0

	struct AuralData
	{
		int timestamp;
		string sender;
		double direction;
		string message;
	};

	struct PlayerParamStruct
	{
		double fValue;
	};

	/*I used a struct here to keep the interfaces consistent. I realize 
		it's not necessary since all it contains is one float. Again, consistancy.*/
	struct PlayerTypeStruct
	{
		double fValue;
	};

	struct SenseBodyData
	{
		int timestamp;

		struct viewModeStruct
		{
			string viewQuality, viewWidth;
			/* NOTE: viewQuality should either be "high" or "low"
					 viewWidth should be "narrow", "normal", or "wide" */
		} view_mode;
		
		Vector2f absLocation;
		Vector2f velocity;

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

	struct ServerStruct
	{
		string sValue;
		double fValue;
	};

	// Visual information that could pertain to any object
	// Visible players include this struct, but player-specific
	// information is stored elsewhere
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

	// Data retrieved from visual messages, but specific to players
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
}

#endif
