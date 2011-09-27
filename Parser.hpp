#ifndef PARSER_HPP
#define PARSER_HPP

/* Contributions by Jared Mar, Keeler Russell, and Corbin Charpentier, all grouped into one big module */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <string.h>
#include <cstdlib>

using std::string;
using std::vector;
using std::tr1::unordered_map;

namespace Parser
{
	#define INVALID_FLOAT_VALUE -50000.0
	#define INVALID_UNIFORM_NUMBER -1
	#define INVALID_TEAM_NAME ""
	#define INVALID_SENDER_NAME ""
	#define INVALID_DIRECTION -50000.0

	struct AuralData
	{
		int timestamp;
		string sender;
		float direction;
		string message;
	};

	struct PlayerParamStruct
	{
		float fValue;
	};

	/*I used a struct here to keep the interfaces consistant. I realize 
		it's not necessary since all it contains is one float. Again, consistancy.*/
	struct PlayerTypeStruct
	{
		float fValue;
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
		
		float stamina[3],
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
			float movable, expires, target[2], count;
		} arm;
		
		struct focusStruct
		{
			string target;
			float count;
		} focus;
		
		struct tackleStruct
		{
			float expires, count;
		} tackle;
		
		string collision;
		
		struct foulStruct
		{
			float charged;
			string card;
		} foul;	
	};

	struct ServerStruct
	{
		string sValue;
		float fValue;
	};

	// Visual information that could pertain to any object
	// Visible players include this struct, but player-specific
	// information is stored elsewhere
	struct VisualData
	{
		int timestamp;

		float distance;
		float direction;
		float distanceChange;
		float directionChange;
	};

	// Data retrieved from visual messages, but specific to players
	struct VisiblePlayer
	{
		std::string teamName;
		int uniformNumber;
		bool isGoalie;

		// Has data about relative position/speed/facing as well
		VisualData visualData;
		float bodyDirection;
		float headDirection;
	};

	bool isBufferComplete( const string buffer );
	void parseAuralPacket( const string auralString, AuralData & auralData );
	void parsePlayerParamPacket( const string buffer, unordered_map<string, PlayerParamStruct> & playerParams );
	void parsePlayerTypePacket( const string buffer, unordered_map<string, PlayerTypeStruct> playerTypes[] );
	void parseSenseBodyPacket( const string inData, SenseBodyData & sbd );
	void parseServerPacket( const string buffer, unordered_map<string, ServerStruct> &serverInfo );
	void parseVisualPacket( const string visualString, unordered_map<string, VisualData> & visualHash,
							vector<VisiblePlayer> & visiblePlayers );
}

#endif
