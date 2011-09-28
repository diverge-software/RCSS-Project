#include "Parser.hpp"
#include <iostream>

using namespace std;
using std::tr1::unordered_map;

bool Parser::isBufferComplete( const string buffer )
{
   const int length = buffer.length();
   int openParenCount = 0;
   int closeParenCount = 0;

   for( int i = 0; i < length; i++ )
   {
	   if( buffer[i] == '(' )
	   {
		   openParenCount++;
	   }
	   else if( buffer[i] == ')' )
	   {
		   closeParenCount++;
	   }
   }

   return ( openParenCount == closeParenCount );
}

void Parser::parseAuralPacket( const string auralString, AuralData & auralData )
{
	char * str = new char[auralString.size() + 1];
	strcpy( str, auralString.c_str() );

	char * val = strtok( str, " ()" );

	// Get the time that the message was sent
	val = strtok( NULL, " ()" );
	auralData.timestamp = atoi( val );

	// Get the sender
	val = strtok( NULL, " ()" );
	if( !strcmp( val, "online_coach_left" ) ||
		!strcmp( val, "online_coach_left" ) ||
		!strcmp( val, "coach" ) ||
		!strcmp( val, "referee" ) ||
		!strcmp( val, "self" )
	  )
	{
		auralData.sender = val;
		auralData.direction = INVALID_DIRECTION;
	}
	// If not a named sender, then direction to sender
	else
	{
		auralData.sender = INVALID_SENDER_NAME;
		auralData.direction = atof( val );
	}

	// Get the message itself
	val = strtok( NULL, "\"()" );
	auralData.message = val;
	delete [] str;
}

void Parser::parseInitPacket( const string initString, int & uniformNumber, char & side )
{
	char * str = new char[initString.size() + 1];
	strcpy( str, initString.c_str() );

	char * val = strtok( str, " ()" );

	// Get the side you're playing from
	val = strtok( NULL, " ()" );
	side = val[0];

	// Get the uniform number
	val = strtok( NULL, " ()" );
	uniformNumber = atoi( val );

	delete [] str;
}

void Parser::parsePlayerParamPacket( const string buffer, unordered_map<string, PlayerParamStruct> & playerParams )
{
	PlayerParamStruct playerParamStruct;
	string key, temp;
	/*Have id; now we're ready to parse the rest of the buffer*/
	for(int i=14; i < buffer.size(); i++)
	{
		key.clear();
		temp.clear();
		if(buffer[i] == '(')
		{
			i++; 									//increment i to next character in buffer
			while(buffer[i] != ' ')					//get head of token; this will also be used as key
			{
				key.push_back(buffer[i]);
				i++;
			}
			i++;
			while(buffer[i] != ')')
			{
				temp.push_back(buffer[i]);
				i++;
			}
			playerParamStruct.fValue = atof(temp.c_str());//convert string to float 	
			playerParams[key] = playerParamStruct;
		}
	} 
}

void Parser::parsePlayerTypePacket( const string buffer, unordered_map<string, PlayerTypeStruct> playerTypes[] )
{
	string key, temp;
	int id;												//used to index playerTypes[].
	int length = buffer.size();
	PlayerTypeStruct playerTypeStruct;

	int i=17;
	/*special case for first token: contains id used as index on unordered_map array*/
	while(buffer[i] != ')')
	{
		temp.push_back(buffer[i]);
		i++;
	}
	id = atoi(temp.c_str());

	/*Have id; now we're ready to parse the rest of the buffer*/
	for(; i<length-1; i++)
	{
		temp.clear();
		key.clear();
		if(buffer[i] == '(')
		{
			i++; 									//increment i to next character in buffer

			while(buffer[i] != ' ')					//get head of token; this will also be used as key
			{
				key.push_back(buffer[i]);
				i++;
			}
			i++;
			while(buffer[i] != ')')
			{
				temp.push_back(buffer[i]);
				i++;
			}
			playerTypeStruct.fValue = atof(temp.c_str());//convert string to float 	
			playerTypes[id][key] = playerTypeStruct;

			//cout << "Key: " << key << " --> playerTypes[" << id << "][" << key << "]: " << playerTypes[id][key].fValue << endl;
		}
	} 
}

void Parser::parseSenseBodyPacket( const string inData, SenseBodyData & sbd )
{	
	// Convert the server data from string to c_string
	char * str = new char[inData.size()+1];
	strcpy (str, inData.c_str());

	// begin separating the string into tokens
	char *val;
	
	val = strtok(str, " ()");
	
	// the first token that comes after "sense_body" is a time stamp
	// get it and store it in the hash table
	val = strtok(NULL, " ()");
	sbd.timestamp = atoi(val);

	//get the next val
	val = strtok(NULL, " ()");
	
	// as long as you haven't reached the end of the string of server data
	while(val !=NULL && val != NULL)
	{
		// if the val is any of the actionCount types
		// get the value and store it in the hash table with the corresponding val
		if(strcmp(val, "head_angle") == 0)
		{
			sbd.head_angle = atof(strtok(NULL, " ()"));
		}
		else if(strcmp(val, "kick") == 0)
		{
			sbd.kick = atof(strtok(NULL, " ()"));
		}
		else if(strcmp(val, "dash") == 0)
		{
			sbd.dash = atof(strtok(NULL, " ()"));
		}
		else if(strcmp(val, "turn") == 0)
		{
			sbd.turn = atof(strtok(NULL, " ()"));
		}
		else if(strcmp(val, "say") == 0)
		{
			sbd.say = atof(strtok(NULL, " ()"));
		}
		else if(strcmp(val, "turn_neck") == 0)
		{
			sbd.turn_neck = atof(strtok(NULL, " ()"));
		}
		else if(strcmp(val, "catch") == 0)
		{
			sbd.catchCount = atof(strtok(NULL, " ()"));
		}
		else if(strcmp(val, "move") == 0)
		{
			sbd.move = atof(strtok(NULL, " ()"));
		}
		else if(strcmp(val, "change_view") == 0)
		{
			sbd.change_view = atof(strtok(NULL, " ()"));
		}
		else if(strcmp(val, "view_mode") == 0)
		{
			sbd.view_mode.viewQuality = strtok(NULL, " ()");
			sbd.view_mode.viewWidth = strtok(NULL, " ()");
		}
		else if(strcmp(val, "stamina") == 0)
		{
			for(int i = 0; i < 3; i++)
			{
				sbd.stamina[i] = atof(strtok(NULL, " ()"));
			}
		}
		else if (strcmp(val, "arm") == 0)
		{
			val = strtok(NULL, " ()");
			sbd.arm.movable = atof(strtok(NULL, " ()"));
			val = strtok(NULL, " ()");
			sbd.arm.expires = atof(strtok(NULL, " ()"));
			val = strtok(NULL, " ()");
			sbd.arm.target[0] = atof(strtok(NULL, " ()"));
			sbd.arm.target[1] = atof(strtok(NULL, " ()"));
			val = strtok(NULL, " ()");
			sbd.arm.count = atof(strtok(NULL, " ()"));
		}
		else if (strcmp(val, "speed") == 0)
		{
			for(int i = 0; i < 2 ; i++)
			{
				sbd.speed[i] = atof(strtok(NULL, " ()"));
			}
		}
		else if (strcmp(val, "tackle") == 0)
		{
			strtok(NULL, " ()");
			sbd.tackle.expires = atof(strtok(NULL, " ()"));
			strtok(NULL, " ()");
			sbd.tackle.count = atof(strtok(NULL, " ()"));
		}
		else if (strcmp(val, "focus") == 0)
		{
			val = strtok(NULL, " ()");
			sbd.focus.target = strtok(NULL, " ()");
			val = strtok(NULL, " ()");
			sbd.focus.count = atof(strtok(NULL, " ()"));
		}
		else if (strcmp(val, "collision") == 0)
		{
			sbd.collision = strtok(NULL, " ()");
		}
		else if (strcmp(val,"foul") == 0)
		{
			strtok(NULL, " ()");
			sbd.foul.charged = atof(strtok(NULL, " ()"));
			strtok(NULL, " ()");
			sbd.foul.card = strtok(NULL, " ()");
		}
		else
		{
			cout << "this key is not recognized.\n";
		}

		val = strtok(NULL, " ()");
	}

	delete [] str;
}

void Parser::parseServerPacket( const string buffer, unordered_map<string, ServerStruct> &serverInfo )
{
	string key;
	ServerStruct serverStruct;

	//cout << buffer << endl;
	int length = buffer.size();
	
	for(int i=14; i<length-1; i++)					//loop through entire string
	{
		if(buffer[i] == '(')						//found a new token
		{
			i++; 									//increment i to next character in buffer

			while(buffer[i] != ' ')					//get head of token; this will also be used as key
			{
				key.push_back(buffer[i]);
				i++;
			}
			/*i is now on a space; value of buffer[i+1] will either be a floating point number or string. Increment.*/
			i++; 
			if(buffer[i] == '"') 					//value must be a string; fill sValue
			{
				i++;								//increment to avoid quotation mark
				while(buffer[i] != '"')
				{
					serverStruct.sValue.push_back(buffer[i]);
					i++;
				}
				serverStruct.fValue = INVALID_FLOAT_VALUE;
				i++;
			}
			else 									//value must be be a floating point number; fill fValue
			{
				string temp;
				while(buffer[i] != ')')
				{
					temp.push_back(buffer[i]);
					i++;
				}
				serverStruct.fValue = atof(temp.c_str());//convert string to float 			
			}	

			/*Now that serverStruct is filled and we have a key, we can populate the private unordered_map instance*/
			serverInfo[key] = serverStruct;
			
			/*clear variables for next token iteration*/
			key.clear();
			serverStruct.sValue.clear();
			serverStruct.fValue = 0;
			
			//rinse and repeat 						
		}
	}
}

void Parser::parseVisualPacket( const string visualString, unordered_map<string, VisualData> & visualHash,
						vector<VisiblePlayer> & visiblePlayers )
{
	// Clear hash and list, will fill with new values
	visualHash.clear();
	visiblePlayers.clear();

	int i = visualString.find( "(", 1 );    // Find the first ( after (see...
	string key;
	string dummyString;    // Holds values we read then convert into more meaningful form
	VisualData dummyVisualData;
	VisiblePlayer dummyPlayerData;

	// Get timestamp
	sscanf( visualString.c_str(), "%*s %d %*s", &dummyVisualData.timestamp );

	while( i < visualString.length() )
	{
		bool isPlayer = false;

		// Scan past whitespace
		if( visualString[i] == ' ' )
		{
			i++;
		}
		// Must have reached the closing paren for the see expression
		if( visualString[i] == ')' )
		{
			break;
		}

		// Get the key value (i.e. the object description)
		i += 2;    // Get past the open parens
		while( visualString[i] != ')' )
		{
			key.push_back( visualString[i] );
			i++;
		}

		// See if this is player data
		if( key[0] == 'p' )
		{
			isPlayer = true;
			dummyPlayerData.teamName = INVALID_TEAM_NAME;
			dummyPlayerData.uniformNumber = INVALID_UNIFORM_NUMBER;
			dummyPlayerData.isGoalie = false;

			char * str = new char[key.size() + 1];
			strcpy( str, key.c_str() );

			char * val = strtok( str, " " );    // Get past the 'p'

			if( key.find( "\"" ) != -1 )
			{
				val = strtok( NULL, "\"" );
				dummyPlayerData.teamName = val;
			}

			// Now handle the uniform number
			val = strtok( NULL, " \"" );
			if( val != NULL && !isalpha( val[0] ) )
			{
				dummyPlayerData.uniformNumber = atoi( val );
			}
			else if( val != NULL && !strcmp( val, "goalie" ) )
			{
				dummyPlayerData.isGoalie = true;
			}

			val = strtok( NULL, " " );
			if( val != NULL && !strcmp( val, "goalie" ) )
			{
				dummyPlayerData.isGoalie = true;
			}

			delete [] str;
		}

		// Get the distance to the object
		i += 2;    // Get past close paren and space
		while( visualString[i] != ' ' )
		{
			dummyString.push_back( visualString[i] );
			i++;
		}
		dummyVisualData.distance = atof( dummyString.c_str() );
		dummyString.clear();

		// Get direction to object
		i++;    // Get past the space
		while( visualString[i] != ' ' && visualString[i] != ')' )
		{
			dummyString.push_back( visualString[i] );
			i++;
		}
		dummyVisualData.direction = atof( dummyString.c_str() );
		dummyString.clear();

		// Hit the end of information about object
		if( visualString[i] == ')' )
		{
			// Store the data
			if( isPlayer )
			{
				dummyVisualData.distanceChange = INVALID_FLOAT_VALUE;
				dummyVisualData.directionChange = INVALID_FLOAT_VALUE;

				dummyPlayerData.headDirection = INVALID_FLOAT_VALUE;
				dummyPlayerData.bodyDirection = INVALID_FLOAT_VALUE;
				dummyPlayerData.visualData = dummyVisualData;

				visiblePlayers.push_back( dummyPlayerData );
			}
			else
			{
				dummyVisualData.directionChange = INVALID_FLOAT_VALUE;
				dummyVisualData.distanceChange = INVALID_FLOAT_VALUE;

				visualHash[key] = dummyVisualData;
			}

			i++;    // Get past the close paren
			key.clear();
			continue;
		}
		if( visualString[i] == ' ' )
		{
			i++;
		}

		// Get the distChng number (see documentation)
		while( visualString[i] != ' ' )
		{
			dummyString.push_back( visualString[i] );
			i++;
		}
		dummyVisualData.distanceChange = atof( dummyString.c_str() );
		dummyString.clear();

		// Get dirChng (see documentation)
		i++;    // Get past the space
		while( visualString[i] != ' ' && visualString[i] != ')' )
		{
			dummyString.push_back( visualString[i] );
			i++;
		}
		dummyVisualData.directionChange = atof( dummyString.c_str() );
		dummyString.clear();

		// Hit the end of information about object
		if( visualString[i] == ')' )
		{
			// Store the data
			if( isPlayer )
			{
				dummyPlayerData.headDirection = INVALID_FLOAT_VALUE;
				dummyPlayerData.bodyDirection = INVALID_FLOAT_VALUE;
				dummyPlayerData.visualData = dummyVisualData;

				visiblePlayers.push_back( dummyPlayerData );
			}
			else
			{
				visualHash[key] = dummyVisualData;
			}

			i++;    // Get past the close paren
			key.clear();
			continue;
		}
		if( visualString[i] == ' ' )
		{
			i++;
		}

		// Only players can have these two extra data items: body and head directions
		if( isPlayer )
		{
			// Get the body direction
			while( visualString[i] != ' ' )
			{
				dummyString.push_back( visualString[i] );
				i++;
			}
			dummyPlayerData.bodyDirection = atof( dummyString.c_str() );
			dummyString.clear();

			// Get the head direction
			i++;    // Get past the space
			while( visualString[i] != ' ' && visualString[i] != ')' )
			{
				dummyString.push_back( visualString[i] );
				i++;
			}
			dummyPlayerData.headDirection = atof( dummyString.c_str() );
			dummyString.clear();

			dummyPlayerData.visualData = dummyVisualData;
			visiblePlayers.push_back( dummyPlayerData );
			i++;    // Get past the close paren
			key.clear();
		}
	}
}

