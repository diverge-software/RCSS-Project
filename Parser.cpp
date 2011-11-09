#include "Parser.hpp"
#include "Debug.hpp"
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
	strcpy_s( str, auralString.size() + 1, auralString.c_str() );

	char * nextToken;
	char * val = strtok_s( str, " ()", &nextToken );

	// Get the time that the message was sent
	val = strtok_s( NULL, " ()", &nextToken );
	auralData.timestamp = atoi( val );

	// Get the sender
	val = strtok_s( NULL, " ()", &nextToken );
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
	val = strtok_s( NULL, "\"()", &nextToken );
	auralData.message = val;
	delete [] str;
}

void Parser::parseInitPacket( const string initString, int & uniformNumber, char & side )
{
	char * str = new char[initString.size() + 1];
	strcpy_s( str, initString.size() + 1, initString.c_str() );

	char * nextToken;
	char * val = strtok_s( str, " ()", &nextToken );

	// Get the side you're playing from
	val = strtok_s( NULL, " ()", &nextToken );
	side = val[0];

	// Get the uniform number
	val = strtok_s( NULL, " ()", &nextToken );
	uniformNumber = atoi( val );

	delete [] str;
}

void Parser::parsePlayerParamPacket( const string buffer, unordered_map<string, PlayerParamStruct> & playerParams )
{
	PlayerParamStruct playerParamStruct;
	string key, temp;
	/*Have id; now we're ready to parse the rest of the buffer*/
	for( unsigned int i=14; i < buffer.size(); i++ )
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
	//flag indicating absolute coords and velocity have not yet been calculated
	sbd.absCalculated = false;

	// Convert the server data from string to c_string
	char * str = new char[inData.size()+1];
	strcpy_s( str, inData.size()+1, inData.c_str() );

	// begin separating the string into tokens
	char * nextToken;
	char * val;
	
	val = strtok_s( str, " ()", &nextToken );
	
	// the first token that comes after "sense_body" is a time stamp
	// get it and store it in the hash table
	val = strtok_s( NULL, " ()", &nextToken);
	sbd.timestamp = atoi(val);

	//get the next val
	val = strtok_s( NULL, " ()", &nextToken );
	
	// as long as you haven't reached the end of the string of server data
	while(val !=NULL && val != NULL)
	{
		// if the val is any of the actionCount types
		// get the value and store it in the hash table with the corresponding val
		if(strcmp(val, "head_angle") == 0)
		{
			sbd.head_angle = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if(strcmp(val, "kick") == 0)
		{
			sbd.kick = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if(strcmp(val, "dash") == 0)
		{
			sbd.dash = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if(strcmp(val, "turn") == 0)
		{
			sbd.turn = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if(strcmp(val, "say") == 0)
		{
			sbd.say = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if(strcmp(val, "turn_neck") == 0)
		{
			sbd.turn_neck = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if(strcmp(val, "catch") == 0)
		{
			sbd.catchCount = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if(strcmp(val, "move") == 0)
		{
			sbd.move = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if(strcmp(val, "change_view") == 0)
		{
			sbd.change_view = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if(strcmp(val, "view_mode") == 0)
		{
			sbd.view_mode.viewQuality = strtok_s( NULL, " ()", &nextToken );
			sbd.view_mode.viewWidth = strtok_s( NULL, " ()", &nextToken );
		}
		else if(strcmp(val, "stamina") == 0)
		{
			for(int i = 0; i < 3; i++)
			{
				sbd.stamina[i] = atof( strtok_s( NULL, " ()", &nextToken ) );
			}
		}
		else if (strcmp(val, "arm") == 0)
		{
			val = strtok_s( NULL, " ()", &nextToken );
			sbd.arm.movable = atof( strtok_s( NULL, " ()", &nextToken ) );
			val = strtok_s( NULL, " ()", &nextToken );
			sbd.arm.expires = atof( strtok_s( NULL, " ()", &nextToken ) );
			val = strtok_s( NULL, " ()", &nextToken );
			sbd.arm.target[0] = atof( strtok_s( NULL, " ()", &nextToken ) );
			sbd.arm.target[1] = atof( strtok_s( NULL, " ()", &nextToken ) );
			val = strtok_s( NULL, " ()", &nextToken );
			sbd.arm.count = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if (strcmp(val, "speed") == 0)
		{
			for(int i = 0; i < 2 ; i++)
			{
				sbd.speed[i] = atof( strtok_s( NULL, " ()", &nextToken ) );
			}
		}
		else if (strcmp(val, "tackle") == 0)
		{
			strtok_s( NULL, " ()", &nextToken );
			sbd.tackle.expires = atof( strtok_s( NULL, " ()", &nextToken ) );
			strtok_s( NULL, " ()", &nextToken );
			sbd.tackle.count = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if (strcmp(val, "focus") == 0)
		{
			val = strtok_s( NULL, " ()", &nextToken );
			sbd.focus.target = strtok_s( NULL, " ()", &nextToken );
			val = strtok_s( NULL, " ()", &nextToken );
			sbd.focus.count = atof( strtok_s( NULL, " ()", &nextToken ) );
		}
		else if (strcmp(val, "collision") == 0)
		{
			sbd.collision = strtok_s( NULL, " ()", &nextToken );
		}
		else if (strcmp(val,"foul") == 0)
		{
			strtok_s( NULL, " ()", &nextToken );
			sbd.foul.charged = atof( strtok_s( NULL, " ()", &nextToken ) );
			strtok_s( NULL, " ()", &nextToken );
			sbd.foul.card = strtok_s( NULL, " ()", &nextToken );
		}
		else
		{
			alwaysAssert();
		}

		val = strtok_s( NULL, " ()", &nextToken );
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
		serverStruct.sValue = INVALID_STRING_VALUE;
		serverStruct.fValue = INVALID_FLOAT_VALUE;

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

	unsigned int i = visualString.find( "(", 1 );    // Find the first ( after (see...
	string key;
	string dummyString;    // Holds values we read then convert into more meaningful form
	VisualData dummyVisualData;
	VisiblePlayer dummyPlayerData;

	// Get timestamp
	sscanf_s( visualString.c_str(), "%*s %d %*s", &dummyVisualData.timestamp );

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
			strcpy_s( str, key.size() + 1, key.c_str() );

			char * nextToken;
			char * val = strtok_s( str, " ", &nextToken );    // Get past the 'p'

			if( key.find( "\"" ) != -1 )
			{
				val = strtok_s( NULL, "\"", &nextToken );
				dummyPlayerData.teamName = val;
			}

			// Now handle the uniform number
			val = strtok_s( NULL, " \"", &nextToken );
			if( val != NULL && !isalpha( val[0] ) )
			{
				dummyPlayerData.uniformNumber = atoi( val );
			}
			else if( val != NULL && !strcmp( val, "goalie" ) )
			{
				dummyPlayerData.isGoalie = true;
			}

			val = strtok_s( NULL, " ", &nextToken );
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

void Parser::convertToAbsoluteCoordsAndVelocity( unordered_map<string, VisualData> &visualHash, 
							vector<VisiblePlayer> &teammateVec, vector<VisiblePlayer> &opponentVec,
							vector<VisiblePlayer> &unidentifiedVec, SenseBodyData &senseBodyData,
							unordered_map<string, Vector2f> &stationaryFlags)
{
	bool ballVisible = false;
	string flags[3]; //Contains keys to the three closest flags used for triangulation.
	int index = 0;
	
	//speed is parsed as speed[0] = the magnitude of the client-player's velocity
	//	and speed[1] = the client-player's location angle relative to the field's x-axis
	//double absoluteSpeedAngle = senseBodyData.speed[1];
	double absoluteSpeedMagnitude = senseBodyData.speed[0];  
	double absAngle = senseBodyData.speed[1];
	double xAvg = 0;
	double yAvg = 0;

	// If speed is velocity is greater than 0, the angle value will be garbage. 
	if(absoluteSpeedMagnitude != 0)
	{
		absAngle = calculateAbsAngle(visualHash);		
	}
	
	// Store absAngle in sensebody info
	senseBodyData.absAngle = absAngle;

	/*
	for(unordered_map<string, VisualData>::const_iterator it = visualHash.begin(); it != visualHash.end(); ++it )
	{	
		//Find three closest flags and ball 
		//NOTE: only need one flat to find absolulute position. Finding three and averaging them
		//		will make the estimate more accurate
		if(it->first[0] == 'f') //it->first returns key at this iteration 
		{	
			if(index < 3)
			{
				flags[index] = it->first;
				index++; 
			}
			else //Compare distances of each flag to find closest flags
			{
				//Find flag with largest distance, replace it with it->first if necessary
				int largesti = 0; //stores index to key associated with largest distance
				for(int i=0; i<index - 1; i++)
				{
					if(visualHash[flags[i+1]].distance > visualHash[flags[largesti]].distance)
					{
						largesti = i+1;
					}
				}
				if(visualHash[flags[largesti]].distance > it->second.distance)
				{
					flags[largesti] = it->first;
				}
			}
		}
	}	
		
	//Find absolute client-player location
	//Takes the average of locations derived from the three flags
	for(int i=0; i<index; i++)
	{
		//AbsoluteAngleSum is the sum of the player's angle with respect to the field and 
		//	the flag's angle with respect to the client-player's
		double absoluteAngleSum = getAbsoluteAngleSum(absAngle, visualHash[flags[i]].direction); 
		
		xAvg += stationaryFlags[flags[i]][0] - visualHash[flags[i]].distance*cos((PI/180)*absoluteAngleSum);
		yAvg += stationaryFlags[flags[i]][1] - visualHash[flags[i]].distance*sin((PI/180)*absoluteAngleSum); 			
	}*/

	
	
	string closestFlag;
	//find a visible flag
	for(unordered_map<string, VisualData>::const_iterator it = visualHash.begin(); it != visualHash.end(); ++it )
	{
		if(it->first[0] == 'f')
		{
			closestFlag = it->first;
			break;
		}
	}

	// Find closest visible flag
	for(unordered_map<string, VisualData>::const_iterator it = visualHash.begin(); it != visualHash.end(); ++it )
	{	
		if(it->first[0] == 'f') //it->first returns key at this iteration 
		{
			if(it->second.distance < visualHash[closestFlag].distance)
			{	
				closestFlag = it->first;
			}
		}
	}
	
	absAngle = getAbsoluteAngleSum(absAngle, visualHash[closestFlag].direction);		

	//Assign client-player's location vales to absLocation vector
	senseBodyData.absLocation[0] = stationaryFlags[closestFlag][0] - visualHash[closestFlag].distance*cos((PI/180)*absAngle);
	senseBodyData.absLocation[1] = stationaryFlags[closestFlag][1] - visualHash[closestFlag].distance*sin((PI/180)*absAngle);	
	
	//convert client-player's absolute velocity to a vector
	senseBodyData.absVelocity[0] = absoluteSpeedMagnitude*cos((PI/180)*absAngle);
	senseBodyData.absVelocity[1] = absoluteSpeedMagnitude*sin((PI/180)*absAngle);
	
	//find absolute ball location
	unordered_map<string, VisualData>::const_iterator findIter = visualHash.find( "b" );
	if(findIter != visualHash.end())
	{
		//calculate absolute position
		double absoluteAngle = getAbsoluteAngleSum(absAngle, visualHash["b"].direction);
		
		visualHash["b"].absLocation[0] = senseBodyData.absLocation[0] + visualHash["b"].distance*cos((PI/180)*absoluteAngle);
		visualHash["b"].absLocation[1] = senseBodyData.absLocation[1] + visualHash["b"].distance*sin((PI/180)*absoluteAngle);
	
		//check if ball is close enough to have observe distance change and direction change
		if( (visualHash["b"].distanceChange != INVALID_FLOAT_VALUE) 
			 && (visualHash["b"].directionChange != INVALID_DIRECTION) )
		{
			//convert ball's velocity to abslolute vector velocity
			double absVecAngle = getAbsoluteAngleSum( absoluteAngle, visualHash["b"].directionChange );

			visualHash["b"].absVelocity[0] = visualHash["b"].distanceChange*cos((PI/180)*absVecAngle)
											 - senseBodyData.absVelocity[0];
			visualHash["b"].absVelocity[1] = visualHash["b"].distanceChange*sin((PI/180)*absVecAngle) 
											 - senseBodyData.absVelocity[1];
		}
		else //invalid
		{
			visualHash["b"].absVelocity = Vector2f(INVALID_FLOAT_VALUE, INVALID_FLOAT_VALUE);
		}		
	}	

	convertAbsPlayersCoords( opponentVec, senseBodyData, absAngle );
	convertAbsPlayersCoords( teammateVec, senseBodyData, absAngle );
	convertAbsPlayersCoords( unidentifiedVec, senseBodyData, absAngle );
}

void Parser::convertAbsPlayersCoords( vector<VisiblePlayer> &playerVec, SenseBodyData &senseBodyData, double absAngle )
{
	if( !playerVec.empty() )
	{
		for(unsigned int i=0; i < playerVec.size(); i++)
		{
			if( (playerVec[i].visualData.distanceChange != INVALID_FLOAT_VALUE) 
				&& (playerVec[i].visualData.directionChange != INVALID_DIRECTION) )
			{
				//calculate absolute position
				double absoluteAngle = getAbsoluteAngleSum(absAngle, playerVec[i].visualData.direction);
			
				playerVec[i].visualData.absLocation[0] = senseBodyData.absLocation[0] + playerVec[i].visualData.distance*cos((PI/180)*absoluteAngle);
				playerVec[i].visualData.absLocation[1] = senseBodyData.absLocation[1] + playerVec[i].visualData.distance*sin((PI/180)*absoluteAngle);
			
				//convert each other player's velocity to absolute vector velocity
				if( (playerVec[i].visualData.distanceChange != INVALID_FLOAT_VALUE) 
					 && (playerVec[i].visualData.directionChange != INVALID_DIRECTION) )
				{ 
					double absVecAngle = getAbsoluteAngleSum( absoluteAngle, playerVec[i].visualData.distanceChange );

					playerVec[i].visualData.absVelocity[0] = playerVec[i].visualData.distanceChange * cos((PI/180)*absVecAngle - senseBodyData.absVelocity[0]);
					playerVec[i].visualData.absVelocity[1] = playerVec[i].visualData.distanceChange * sin((PI/180)*absVecAngle - senseBodyData.absVelocity[1]);
				}
				else //set values to invalid
				{
					playerVec[i].visualData.absVelocity = Vector2f(INVALID_FLOAT_VALUE, INVALID_FLOAT_VALUE);
				}
			}		
		}
	}
}

double Parser::calculateAbsAngle(unordered_map<string, VisualData> &visualHash)
{
	double angle = 0;
	string lineName;
	VisualData tVisualData;

	//find a line
	for( unordered_map<string, VisualData>::reverse_iterator it = visualHash.rbegin(); it != visualHash.rend(); ++it )
	{
		if( it->first[0] == 'l' )
		{
			angle = it->second.direction;
			lineName = it->first;
			break;
		}
	}

	if( !lineName.compare( 0, 3, "l t" ) )	
	{
		angle -= 180;
	}
	else if( !lineName.compare( 0, 3, "l r" ) )	
	{
		angle -= 90;
	}
	else if( !lineName.compare( 0, 3, "l l" ) )	
	{
		angle += 90;
	}
	else //line must bet bottom	
	{
		angle += 180;
	}
	
	return angle;
}

double Parser::getAbsoluteAngleSum(double absAngle, double refAngle)
{
	double absoluteAngleSum = absAngle + refAngle;
	if(absoluteAngleSum > 180)
	{
			absoluteAngleSum -= 360;
	}
	else if(absoluteAngleSum < -180)
	{
		absoluteAngleSum += 360;
	}

	//server is retarded
	return (-1)*absoluteAngleSum;
}

vector<Parser::VisiblePlayer> Parser::getTeammateIdentities( string teamName, const vector<VisiblePlayer> & visiblePlayers )
{
	vector<Parser::VisiblePlayer> teammateList;

	for( unsigned int i = 0; i < visiblePlayers.size(); i++ )
	{
		if( visiblePlayers[i].teamName == teamName )
		{
			teammateList.push_back( visiblePlayers[i] );
		}
	}

	return teammateList;
}

vector<Parser::VisiblePlayer> Parser::getOpponentIdentities( string teamName, const vector<VisiblePlayer> & visiblePlayers )
{
	vector<Parser::VisiblePlayer> opponentList;

	for( unsigned int i = 0; i < visiblePlayers.size(); i++ )
	{
		if( visiblePlayers[i].teamName != teamName &&
			visiblePlayers[i].teamName != INVALID_TEAM_NAME )
		{
			opponentList.push_back( visiblePlayers[i] );
		}
	}

	return opponentList;
}

vector<Parser::VisiblePlayer> Parser::getUnidentifiedIdentities( string teamName, const vector<VisiblePlayer> & visiblePlayers )
{
	vector<Parser::VisiblePlayer> unidentifiedList;

	for( unsigned int i = 0; i < visiblePlayers.size(); i++ )
	{
		if( visiblePlayers[i].teamName == INVALID_TEAM_NAME )
		{
			unidentifiedList.push_back( visiblePlayers[i] );
		}
	}

	return unidentifiedList;
}
