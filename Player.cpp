#include "Player.hpp"
#include "Parser.hpp"
#include "Debug.hpp"

#include <iostream>
using std::cout;
using std::endl;

// Used for stationary flags
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

// Initialize internal structs to invalid values
Player::Player()
{
	AuralData auralData;
	auralData.timestamp = -1;
	auralData.sender = INVALID_SENDER_NAME;
	auralData.direction = INVALID_FLOAT_VALUE;
	auralData.message = INVALID_STRING_VALUE;
	mAuralDataQueue.push_back( auralData );

	SenseBodyData senseBodyData;
	senseBodyData.timestamp = -1;
	senseBodyData.view_mode.viewQuality = INVALID_STRING_VALUE;
	senseBodyData.view_mode.viewWidth = INVALID_STRING_VALUE;
	senseBodyData.stamina[0] = INVALID_FLOAT_VALUE;
	senseBodyData.stamina[1] = INVALID_FLOAT_VALUE;
	senseBodyData.stamina[2] = INVALID_FLOAT_VALUE;
	senseBodyData.speed[0] = INVALID_FLOAT_VALUE;
	senseBodyData.speed[1] = INVALID_FLOAT_VALUE;
	senseBodyData.head_angle = INVALID_FLOAT_VALUE;
	senseBodyData.kick = INVALID_FLOAT_VALUE;
	senseBodyData.dash = INVALID_FLOAT_VALUE;
	senseBodyData.turn = INVALID_FLOAT_VALUE;
	senseBodyData.say = INVALID_FLOAT_VALUE;
	senseBodyData.turn_neck = INVALID_FLOAT_VALUE;
	senseBodyData.catchCount = INVALID_FLOAT_VALUE;
	senseBodyData.move = INVALID_FLOAT_VALUE;
	senseBodyData.change_view = INVALID_FLOAT_VALUE;
	senseBodyData.arm.movable = INVALID_FLOAT_VALUE;
	senseBodyData.arm.expires = INVALID_FLOAT_VALUE;
	senseBodyData.arm.target[0] = INVALID_FLOAT_VALUE;
	senseBodyData.arm.target[1] = INVALID_FLOAT_VALUE;
	senseBodyData.arm.count = INVALID_FLOAT_VALUE;
	senseBodyData.focus.target = INVALID_STRING_VALUE;
	senseBodyData.focus.count = INVALID_FLOAT_VALUE;
	senseBodyData.tackle.expires = INVALID_FLOAT_VALUE;
	senseBodyData.tackle.count = INVALID_FLOAT_VALUE;
	senseBodyData.collision = INVALID_STRING_VALUE;
	senseBodyData.foul.charged = INVALID_FLOAT_VALUE;
	senseBodyData.foul.card = INVALID_STRING_VALUE;
	mSenseBodyDataQueue.push_back( senseBodyData );

	mStationaryFlags["g l"] = Vector2f( LEFT_LINE_X, 0 );
	mStationaryFlags["g r"] = Vector2f( RIGHT_LINE_X, 0 );
	mStationaryFlags["f c"] = Vector2f( 0.0f, 0.0f );
	mStationaryFlags["f c t"] = Vector2f( 0.0f, TOP_LINE_Y );
	mStationaryFlags["f c b"] = Vector2f( 0.0f, BOTTOM_LINE_Y );
	mStationaryFlags["f l t"] = Vector2f( LEFT_LINE_X, TOP_LINE_Y );
	mStationaryFlags["f l b"] = Vector2f( LEFT_LINE_X, BOTTOM_LINE_Y );
	mStationaryFlags["f r t"] = Vector2f( RIGHT_LINE_X, TOP_LINE_Y );
	mStationaryFlags["f r b"] = Vector2f( RIGHT_LINE_X, BOTTOM_LINE_Y );
	mStationaryFlags["f p l t"] = Vector2f( PENALTY_LEFT, PENALTY_TOP );
	mStationaryFlags["f p l b"] = Vector2f( PENALTY_LEFT, PENALTY_BOTTOM );
	mStationaryFlags["f p r t"] = Vector2f( PENALTY_RIGHT, PENALTY_TOP );
	mStationaryFlags["f p r b"] = Vector2f( PENALTY_RIGHT, PENALTY_BOTTOM );
	mStationaryFlags["f g l t"] = Vector2f( LEFT_LINE_X, GOALPOST_TOP_Y );
	mStationaryFlags["f g l b"] = Vector2f( LEFT_LINE_X, GOALPOST_BOTTOM_Y );
	mStationaryFlags["f g r t"] = Vector2f( RIGHT_LINE_X, GOALPOST_TOP_Y );
	mStationaryFlags["f g r b"] = Vector2f( RIGHT_LINE_X, GOALPOST_BOTTOM_Y );
	mStationaryFlags["f l 0"] = Vector2f( LEFT_BOUNDARY_X, 0.0f );
	mStationaryFlags["f r 0"] = Vector2f( RIGHT_BOUNDARY_X, 0.0f );
	mStationaryFlags["f t 0"] = Vector2f( 0.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f b 0"] = Vector2f( 0.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f t l 10"] = Vector2f( -10.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f t l 20"] = Vector2f( -20.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f t l 30"] = Vector2f( -30.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f t l 40"] = Vector2f( -40.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f t l 50"] = Vector2f( -50.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f t r 10"] = Vector2f( 10.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f t r 20"] = Vector2f( 20.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f t r 30"] = Vector2f( 30.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f t r 40"] = Vector2f( 40.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f t r 50"] = Vector2f( 50.0f, TOP_BOUNDARY_Y );
	mStationaryFlags["f b l 10"] = Vector2f( -10.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f b l 20"] = Vector2f( -20.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f b l 30"] = Vector2f( -30.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f b l 40"] = Vector2f( -40.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f b l 50"] = Vector2f( -50.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f b r 10"] = Vector2f( 10.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f b r 20"] = Vector2f( 20.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f b r 30"] = Vector2f( 30.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f b r 40"] = Vector2f( 40.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f b r 50"] = Vector2f( 50.0f, BOTTOM_BOUNDARY_Y );
	mStationaryFlags["f l t 10"] = Vector2f( LEFT_BOUNDARY_X, 10.0f );
	mStationaryFlags["f l t 20"] = Vector2f( LEFT_BOUNDARY_X, 20.0f );
	mStationaryFlags["f l t 30"] = Vector2f( LEFT_BOUNDARY_X, 30.0f );
	mStationaryFlags["f l b 10"] = Vector2f( LEFT_BOUNDARY_X, -10.0f );
	mStationaryFlags["f l b 20"] = Vector2f( LEFT_BOUNDARY_X, -20.0f );
	mStationaryFlags["f l b 30"] = Vector2f( LEFT_BOUNDARY_X, -30.0f );
	mStationaryFlags["f r t 10"] = Vector2f( RIGHT_BOUNDARY_X, 10.0f );
	mStationaryFlags["f r t 20"] = Vector2f( RIGHT_BOUNDARY_X, 20.0f );
	mStationaryFlags["f r t 30"] = Vector2f( RIGHT_BOUNDARY_X, 30.0f );
	mStationaryFlags["f r b 10"] = Vector2f( RIGHT_BOUNDARY_X, -10.0f );
	mStationaryFlags["f r b 20"] = Vector2f( RIGHT_BOUNDARY_X, -20.0f );
	mStationaryFlags["f r b 30"] = Vector2f( RIGHT_BOUNDARY_X, -30.0f );
}

// Decide the buffer type and pass to respective parsing function
bool Player::parseBuffer(const string buffer)
{
	if( isBufferComplete( buffer ) )
	{
		// Things parsed more often are put near top
		if( !buffer.compare( 0, 4, "(see" ) )
		{
			vector<VisiblePlayer> playerList;
			unordered_map<string, VisualData> visualData;
			parseVisualPacket( buffer, visualData, playerList );
			
			if(mSenseBodyDataQueue.empty())
			{
				//convert to abslolute coordinates and velocities
				convertToAbsoluteCoordsAndVelocity( visualData, playerList, mSenseBodyDataQueue.back(), mStationaryFlags);
			}

					deque<vector<VisiblePlayer>>			  mTeammateListQueue;
		deque<vector<VisiblePlayer>>			  mOpponentListQueue;
		deque<vector<VisiblePlayer>>			  mUnidentifiedPlayerListQueue;
			vector<VisiblePlayer> teammates = getTeammateIdentities( this->teamName, playerList );
			vector<VisiblePlayer> opponents = getOpponentIdentities( this->teamName, playerList );
			vector<VisiblePlayer> unidentified = getUnidentifiedIdentities( this->teamName, playerList );

			// Remove the oldest visible teamamte list if necessary,
			// then push the new one onto the back
			if( mTeammateListQueue.size() >= MAX_QUEUE_SIZE )
			{
				mTeammateListQueue.pop_front();
			}
			mTeammateListQueue.push_back( teammates );
			// Remove the oldest visible opponent list if necessary,
			// then push the new one onto the back
			if( mOpponentListQueue.size() >= MAX_QUEUE_SIZE )
			{
				mOpponentListQueue.pop_front();
			}
			mOpponentListQueue.push_back( opponents );
			// Remove the oldest visible unidentified list if necessary,
			// then push the new one onto the back
			if( mUnidentifiedPlayerListQueue.size() >= MAX_QUEUE_SIZE )
			{
				mUnidentifiedPlayerListQueue.pop_front();
			}
			mUnidentifiedPlayerListQueue.push_back( unidentified );

			// Remove the oldest visual data hash if necessary,
			// then push the new one onto the back
			if( mVisualDataQueue.size() >= MAX_QUEUE_SIZE )
			{
				mVisualDataQueue.pop_front();
			}
			mVisualDataQueue.push_back( visualData );
		}
		else if( !buffer.compare( 0, 11, "(sense_body" ) )
		{
			SenseBodyData senseBodyData;
			parseSenseBodyPacket( buffer, senseBodyData );

			// Remove the oldest sense body struct if necessary,
			// then push the new one onto the back
			if( mSenseBodyDataQueue.size() >= MAX_QUEUE_SIZE )
			{
				mSenseBodyDataQueue.pop_front();
			}
			mSenseBodyDataQueue.push_back( senseBodyData );
		}
		else if( !buffer.compare( 0, 5, "(hear" ) )
		{
			AuralData auralData;
			parseAuralPacket( buffer, auralData );

			// Remove the oldest aural data struct if necessary,
			// then push the new one onto the back
			if( mAuralDataQueue.size() >= MAX_QUEUE_SIZE )
			{
				mAuralDataQueue.pop_front();
			}
			mAuralDataQueue.push_back( auralData );
		}
		else if( !buffer.compare( 0, 5, "(init" ) )
		{
			parseInitPacket( buffer, this->uniformNumber, this->side );
		}
		else if( !buffer.compare( 0, 13, "(server_param" ) )
		{
			parseServerPacket( buffer, this->mServerInfo );
		}
		else if( !buffer.compare( 0, 12, "(player_type" ) ) 
		{
			parsePlayerTypePacket( buffer, this->mPlayerTypes );
		}
		else if( !buffer.compare( 0, 13, "(player_param" ) )
		{
			parsePlayerParamPacket( buffer, this->mPlayerParams );
		}
		else if( !buffer.compare( 0, 6, "(error" ) )
		{
            cout << "Error parsing this: " << buffer << endl;
			alwaysAssert();
		}
		else
		{
			cout << "Error parsing this: " << buffer << endl;
			softAssert( false );    // Always soft asserts
		}

		return true;
	}
	else
	{
		return false;
	}	
}

void Player::printNewestVisualHash( ostream & os ) const
{
	os << "############################################" << endl;
	os << "##            Visual Information          ##" << endl;
	os << "############################################" << endl;

	// Get the most recent element and print it
	unordered_map<string, VisualData> visualData = mVisualDataQueue.back();

	for( unordered_map<string, VisualData>::const_iterator it = visualData.begin(); it != visualData.end(); ++it )
	{
		os << "[\"" << it->first << "\", " << it->second.distance << ", " << it->second.direction;
		if( it->second.distanceChange != INVALID_FLOAT_VALUE )
		{
			os << ", " << it->second.distanceChange << ", " << it->second.directionChange << "]" << endl;
		}
		else
		{
			os << "]" << endl;
		}
	}
}

void Player::printServerHash( ostream & os ) const
{
	os << "############################################" << endl;
	os << "##            Server Information          ##" << endl;
	os << "############################################" << endl;
	for( unordered_map<string, ServerStruct>::const_iterator it = mServerInfo.begin(); it != mServerInfo.end(); ++it )
	{
		if( it->second.fValue == INVALID_FLOAT_VALUE )
		{
			os << "[\"" << it->first << "\", " << it->second.sValue << "]" << endl;
		}
		else
		{
			os << "[\"" << it->first << "\", " << it->second.fValue << "]" << endl;
		}
	}
}

void Player::printPlayerTypesHash( ostream & os ) const
{
	os << "############################################" << endl;
	os << "##         Player Type Information        ##" << endl;
	os << "############################################" << endl;
	for( int i = 0; i < NUM_PLAYER_TYPES; i++ )
	{
		os << "PLAYER TYPE #" << i << endl;
		for( unordered_map<string, PlayerTypeStruct>::const_iterator it = mPlayerTypes[i].begin(); it != mPlayerTypes[i].end(); ++it )
		{
			os << "[\"" << it->first << "\", " << it->second.fValue << "]" << endl;
		}
	}
}

void Player::printNewestVisiblePlayersList( ostream & os ) const
{
	os << "############################################" << endl;
	os << "##            Player Information          ##" << endl;
	os << "############################################" << endl;

	vector<VisiblePlayer> players[3];
	// Get the most recent player list and print it
	players[0] = mTeammateListQueue.back();
	players[1] = mOpponentListQueue.back();
	players[2] = mUnidentifiedPlayerListQueue.back();

	for( int i = 0; i < 3; i++ )
	{
		for( unsigned int j = 0; j < players[i].size(); j++ )
		{
			os << "[p, teamName = ";
			if( players[i][j].teamName != INVALID_TEAM_NAME )
			{
				os << players[i][j].teamName;
			}
			else
			{
				os << "UNKNOWN";
			}
			if( players[i][j].uniformNumber != INVALID_UNIFORM_NUMBER )
			{
				os << ", uniform = " << players[i][j].uniformNumber;
			}
			else
			{
				os << ", uniform = UNKNOWN";
			}
			if( players[i][j].isGoalie )
			{
				os << ", is goalie";
			}
			
			os << ", absLocation[0]" << players[i][j].visualData.absLocation[0]
		       << ", absLocation[1]" << players[i][j].visualData.absLocation[1]
			   << ", absVelocity[0]" << players[i][j].visualData.absVelocity[0]
			   << ", absVelocity[1]" << players[i][j].visualData.absVelocity[1];

			os << ", " << players[i][j].visualData.distance << ", " << players[i][j].visualData.direction;

			if( players[i][j].visualData.directionChange != INVALID_FLOAT_VALUE )
			{
				os << ", " << players[i][j].visualData.distanceChange << ", " << players[i][j].visualData.directionChange;
				if( players[i][j].bodyDirection != INVALID_FLOAT_VALUE )
				{
					os << ", " << players[i][j].bodyDirection << ", " << players[i][j].headDirection;
				}
				os << "]" << endl;
			}
			else
			{
				os << "]" << endl;
			}
		}
	}
}

void Player::printNewestAuralStruct( ostream & os ) const
{
	os << "############################################" << endl;
	os << "##            Aural Information           ##" << endl;
	os << "############################################" << endl;

	// Get the most recent auralData and print it
	AuralData auralData = mAuralDataQueue.back();

	os << "Time: " << auralData.timestamp << endl;

	if( auralData.sender != INVALID_SENDER_NAME )
	{
		os << "Sender: " << auralData.sender << endl;
	}
	else
	{
		os << "Src Direction: " << auralData.direction << endl;
	}

	os << "Message: " << auralData.message << endl;
}

void Player::printNewestSenseBodyStruct( ostream & os ) const
{
	os << "############################################" << endl;
	os << "##          Sense Body Information        ##" << endl;
	os << "############################################" << endl;

	// Get most recent sense body data and print it
	SenseBodyData senseBodyData = mSenseBodyDataQueue.back();

	os << "timestamp" << ": " << senseBodyData.timestamp << endl;
	
	os << "view_mode" << ": " << senseBodyData.view_mode.viewQuality << " "
	                            << senseBodyData.view_mode.viewWidth << endl;

	os << "absLocation[0]" << ": " << senseBodyData.absLocation[0] << endl
	   << "absLocation[1]" << ": " << senseBodyData.absLocation[1] << endl;

	os << "absVelocity[0]" << ": " << senseBodyData.absVelocity[0] << endl
	   << "absVelocity[1]" << ": " << senseBodyData.absVelocity[1] << endl;
	 
	
	os << "stamina" << ": " << senseBodyData.stamina[0] << " "
	                          << senseBodyData.stamina[1] << " "
	                          << senseBodyData.stamina[2] << endl;
	
	os << "speed" << ": " << senseBodyData.speed[0] << " "
	                        << senseBodyData.speed[1] << endl;
	
	os << "head_angle" << ": " << senseBodyData.head_angle << endl;
	
	os << "kick" << ": " << senseBodyData.kick << endl;
	
	os << "dash" << ": " << senseBodyData.dash << endl;
	
	os << "turn" << ": " << senseBodyData.turn << endl;
	
	os << "say" << ": " << senseBodyData.say << endl;
	
	os << "turn_neck" << ": " << senseBodyData.turn_neck << endl;
	
	os << "catch" << ": " << senseBodyData.catchCount << endl;
	
	os << "move" << ": " << senseBodyData.move << endl;
	
	os << "change_view" << ": " << senseBodyData.change_view << endl;
	
	os << "arm" << ": " << senseBodyData.arm.movable << " "
	                      << senseBodyData.arm.expires << " "
	                      << senseBodyData.arm.target[0] << " "
	                      << senseBodyData.arm.target[1] << " "
	                      << senseBodyData.arm.count << endl;
	
	os << "focus" << ": " << senseBodyData.focus.target << " "
	                        << senseBodyData.focus.count << endl;
	
	os << "tackle" << ": " << senseBodyData.tackle.expires << " "
	                         << senseBodyData.tackle.count << endl;
	
	os << "collision" << ": " << senseBodyData.collision << endl;
	
	os << "foul" << ": " << senseBodyData.foul.charged << " "
	                       << senseBodyData.foul.card << endl;
}

void Player::printPlayerParamHash( ostream & os ) const
{
	os << "############################################" << endl;
	os << "##         Player Param Information       ##" << endl;
	os << "############################################" << endl;
	for( int i = 0; i < 10; i++ )
	{
		for( unordered_map<string, PlayerParamStruct>::const_iterator it = mPlayerParams.begin(); it != mPlayerParams.end(); ++it )
		{
			os << "[\"" << it->first << "\", " << it->second.fValue << "]" << endl;
		}
	}
}

Vector2f Player::getObjectPosition( string objName, int currentTimestamp ) const
{
	// If the object is a flag, we already know its position, so return it
	if( objName[0] == 'f' || objName[0] == 'g' )
	{
		unordered_map<string, Vector2f>::const_iterator it = mStationaryFlags.find( objName );
		if( it != mStationaryFlags.end() )
		{
			return it->second;
		}
		else
		{
			cout << "Unknown flag name: " << objName << endl;
			alwaysAssert();
		}
	}

	// Could be in most recent visual data hash, try searching there
	unordered_map<string, VisualData> latestVisualHash = mVisualDataQueue.back();
	unordered_map<string, VisualData>::const_iterator latestIt = latestVisualHash.begin();
	if( latestIt->second.timestamp == currentTimestamp )
	{
		unordered_map<string, VisualData>::const_iterator it = latestVisualHash.find( objName );
		if( it != latestVisualHash.end() )
		{
			return it->second.absLocation;
		}
	}

	// Otherwise, we need to estimate its position
	// Traverse backwards through the queue, since the newest data is toward the back
	Vector2f result( INVALID_FLOAT_VALUE, INVALID_FLOAT_VALUE );
	for( unsigned int i = mVisualDataQueue.size() - 1; i >= 0; i-- )
	{
		// Try to find the object in the currently considered visualHash
		unordered_map<string, VisualData>::const_iterator findIter = mVisualDataQueue[i].find( objName );
		// If it has been found, then estimate the position of the object
		if( findIter != mVisualDataQueue[i].end() )
		{
			int oldTimestamp = findIter->second.timestamp;
			Vector2f oldPosition = findIter->second.absLocation;
			Vector2f oldVelocity = findIter->second.absVelocity;

			// Use linear estimation, although the server seems to exhibit friction
			result = oldPosition + oldVelocity * ( currentTimestamp - oldTimestamp );
			break;
		}
	}

	return result;
}

void Player::setTeamName(string teamname)
{
	teamName = teamname;
}

void Player::setPlayerRole( string role )
{
	if( role == "goalie" ||
		role == "defender" ||
		role == "midfielder" ||
		role == "forward" )
	{
		playerRole = role;
	}
	else
	{
		cout << "Player role cannot be: " << role << endl;
		alwaysAssert();
	}
}
