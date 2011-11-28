#include "Player.hpp"
#include "Parser.hpp"
#include "Debug.hpp"
#include <sstream>

#include <iostream>
using std::cout;
using std::endl;
using namespace AI_Processing;

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
	mStationaryFlags["g r"] = Vector2f( RIGHT_LINE_X, 0.0f );
	mStationaryFlags["f c"] = Vector2f( 0.0f, 0.0f );
	mStationaryFlags["f c t"] = Vector2f( 0.0f, TOP_LINE_Y );
	mStationaryFlags["f c b"] = Vector2f( 0.0f, BOTTOM_LINE_Y );
	mStationaryFlags["f l t"] = Vector2f( LEFT_LINE_X, TOP_LINE_Y );
	mStationaryFlags["f l b"] = Vector2f( LEFT_LINE_X, BOTTOM_LINE_Y );
	mStationaryFlags["f r t"] = Vector2f( RIGHT_LINE_X, TOP_LINE_Y );
	mStationaryFlags["f r b"] = Vector2f( RIGHT_LINE_X, BOTTOM_LINE_Y );
	mStationaryFlags["f p l t"] = Vector2f( PENALTY_LEFT, PENALTY_TOP );
	mStationaryFlags["f p l b"] = Vector2f( PENALTY_LEFT, PENALTY_BOTTOM );
	mStationaryFlags["f p l c"] = Vector2f( PENALTY_LEFT, 0.0f );
	mStationaryFlags["f p r t"] = Vector2f( PENALTY_RIGHT, PENALTY_TOP );
	mStationaryFlags["f p r b"] = Vector2f( PENALTY_RIGHT, PENALTY_BOTTOM );
	mStationaryFlags["f p r c"] = Vector2f( PENALTY_RIGHT, 0.0f );
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

	//initialize states
	playerInitialized	 = false;
	servInitialized		 = false; 
	kickOffMode			 = false;
	clientPossessesBall  = false;
	playOn				 = false;
	freeKickFriendly	 = false;
	freeKickOpponent	 = false;
	teamPossessesBall	 = false;

	dashAfterTurnMode	 = false;
	goalieInPosition	 = false;
	viewModeSet			 = false;
	
	uNumPossessesBall	 = INVALID_UNIFORM_NUMBER;
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

			vector<VisiblePlayer> teammates = getTeammateIdentities( this->teamName, playerList );
			vector<VisiblePlayer> opponents = getOpponentIdentities( this->teamName, playerList );
			vector<VisiblePlayer> unidentified = getUnidentifiedIdentities( this->teamName, playerList );

			// Remove the oldest visible teamamte list if necessary,
			// then push the new one onto the back
			if( mTeammateListQueue.size() >= MAX_QUEUE_SIZE )
			{
				mTeammateListQueue.pop_front();
			}
			if( !teammates.empty() )
			{
				mTeammateListQueue.push_back( teammates );
			}

			// Remove the oldest visible opponent list if necessary,
			// then push the new one onto the back
			if( mOpponentListQueue.size() >= MAX_QUEUE_SIZE )
			{
				mOpponentListQueue.pop_front();
			}
			if( !opponents.empty() )
			{
				mOpponentListQueue.push_back( opponents );
			}

			// Remove the oldest visible unidentified list if necessary,
			// then push the new one onto the back
			if( mUnidentifiedPlayerListQueue.size() >= MAX_QUEUE_SIZE )
			{
				mUnidentifiedPlayerListQueue.pop_front();
			}
			if( !unidentified.empty() )
			{
				mUnidentifiedPlayerListQueue.push_back( unidentified );
			}

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

			// Convert to Absolute Coordinates
			if( !mVisualDataQueue.empty())
			{
				vector<VisiblePlayer> teammates;
				vector<VisiblePlayer> opponents;
				vector<VisiblePlayer> unidentified;
				if( !mTeammateListQueue.empty() )
				{
					teammates = mTeammateListQueue.back();
					mTeammateListQueue.pop_back();
				}
				if( !mOpponentListQueue.empty() )
				{
					opponents = mOpponentListQueue.back();
					mOpponentListQueue.pop_back();
				}
				if( !mUnidentifiedPlayerListQueue.empty() )
				{
					unidentified = mUnidentifiedPlayerListQueue.back();
					mUnidentifiedPlayerListQueue.pop_back();
				}
	
				convertToAbsoluteCoordsAndVelocity( mVisualDataQueue.back(), teammates, opponents, 
													unidentified, senseBodyData, mStationaryFlags);	
				if( !teammates.empty() )
				{
					mTeammateListQueue.push_back( teammates );
				}
				if( !opponents.empty() )
				{
					mOpponentListQueue.push_back( opponents );
				}
				if( !unidentified.empty() )
				{
					mUnidentifiedPlayerListQueue.push_back( unidentified );
				}
			}

			mSenseBodyDataQueue.push_back( senseBodyData );
		}
		else if( !buffer.compare( 0, 5, "(hear" ) )
		{
			string temp;
			AuralData auralData;
			parseAuralPacket( buffer, auralData );

			// Remove the oldest aural data struct if necessary,
			// then push the new one onto the back
			if( mAuralDataQueue.size() >= MAX_QUEUE_SIZE )
			{
				mAuralDataQueue.pop_front();
			}
			mAuralDataQueue.push_back( auralData );

			if( auralData.sender.compare( "referee" ) == 0)
			{//asdfasdfa
				playMode = auralData.message;
			}

			if( auralData.message.compare( 0, 1, "t" ) )
			{
				teamPossessesBall = true;
				for( unsigned int i = 1; i < auralData.message.size(); i++ )
				{
					temp.push_back( auralData.message[i] ); 
					uNumPossessesBall = atoi(temp.c_str());
				} 
			} 
			else if( auralData.message.compare( 0, 1, "o" ) )
			{
				teamPossessesBall = true;
				for( unsigned int i = 1; i < auralData.message.size(); i++ )
				{
					temp.push_back( auralData.message[i] ); 
					uNumPossessesBall = atoi(temp.c_str());
				} 
			}

		}
		else if( !buffer.compare( 0, 5, "(init" ) )
		{
			// Offline trainers receive a special "(init ok)" message when the
			// server is ready for them, so deal with it specially.
			if( !buffer.compare( 5, 4, " ok)" ) && playerRole == PLAYER_TYPE_TRAINER )
			{
				uniformNumber = 12;
				side = 'n';    // n for no side
			}
			else
			{
				parseInitPacket( buffer, this->uniformNumber, this->side );
			}

			playerInitialized = true;
		}
		else if( !buffer.compare( 0, 13, "(server_param" ) )
		{
			parseServerPacket( buffer, this->mServerInfo );
			servInitialized = true;
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
            if( this->playerRole == PLAYER_TYPE_TRAINER )
                {
                cout << "Trainer: " << buffer << endl;
                }
            else
                {
                cout << "Error parsing this: " << buffer << endl;
			    alwaysAssert();
                }
		}
		else
		{
            if( this->playerRole == PLAYER_TYPE_TRAINER )
                {
                cout << "Trainer: " << buffer << endl;
                }
            else
                {
			    cout << "Error parsing this: " << buffer << endl;
			    softAssert( false );
                }
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
	if( !mVisualDataQueue.empty() )
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

		// Print ball info
		os << "Ball.absLocation[0]: " << visualData["b"].absLocation[0] << endl
		   << "Ball.absLocation[1]: " << visualData["b"].absLocation[1] << endl
		   << "Ball.absVelocity[0]: " << visualData["b"].absVelocity[0] << endl
		   << "Ball.absVelocity[1]: " << visualData["b"].absVelocity[1] << endl;
	}
}

void Player::printServerHash( ostream & os ) const
{
	if( servInitialized )
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
}

void Player::printPlayerTypesHash( ostream & os ) const
{
	if( playerInitialized )
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
}

void Player::printNewestVisiblePlayersList( ostream & os ) const
{

	os << "############################################" << endl;
	os << "##            Player Information          ##" << endl;
	os << "############################################" << endl;

	// Get the most recent player list and print it

	if( !mTeammateListQueue.empty() )
	{	
		os << "###### TEAMMATES ######" << endl;
		printNewestVisiblePlayersListHelper(os, mTeammateListQueue.back());		
	}
	if( !mOpponentListQueue.empty() )
	{
		os << "###### OPPONENTS ######" << endl;
		printNewestVisiblePlayersListHelper(os, mOpponentListQueue.back());
	}
	if( !mUnidentifiedPlayerListQueue.empty() )
	{
		os << "###### UNIDENTIFIED ######" << endl;
		printNewestVisiblePlayersListHelper(os, mUnidentifiedPlayerListQueue.back());
	}
}

void Player::printNewestVisiblePlayersListHelper( ostream & os, vector<VisiblePlayer> players ) const
{
	for( unsigned int j = 0; j < players.size(); j++ )
	{
		os << "[p, teamName = ";
		if( players[j].teamName != INVALID_TEAM_NAME )
		{
			os << players[j].teamName;
		}
		else
		{
			os << "UNKNOWN";
		}
		if( players[j].uniformNumber != INVALID_UNIFORM_NUMBER )
		{
			os << ", uniform = " << players[j].uniformNumber;
		}
		else
		{
			os << ", uniform = UNKNOWN";
		}
		if( players[j].isGoalie )
		{
				os << ", is goalie";
		}
				
		os << endl << "absLocation[0]: " << players[j].visualData.absLocation[0] << endl
		   << "absLocation[1]: " << players[j].visualData.absLocation[1] << endl
		   << "absVelocity[0]: " << players[j].visualData.absVelocity[0] << endl
		   << "absVelocity[1]: " << players[j].visualData.absVelocity[1] << endl;

		os << "Distanc: " << players[j].visualData.distance << ", " << "Direction: " << players[j].visualData.direction << endl;;

		if( players[j].visualData.directionChange != INVALID_FLOAT_VALUE )
		{
			os << ", " << players[j].visualData.distanceChange << ", " << players[j].visualData.directionChange;
			if( players[j].bodyDirection != INVALID_FLOAT_VALUE )
			{
				os << ", " << players[j].bodyDirection << ", " << players[j].headDirection;
			}
			os << "]" << endl;
		}
		else
		{
			os << "]" << endl;
		}
	}
}

void Player::printNewestAuralStruct( ostream & os ) const
{
	if( !mAuralDataQueue.empty() )
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
}

void Player::printNewestSenseBodyStruct( ostream & os ) const
{

	if( !mSenseBodyDataQueue.empty() )
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
}

void Player::printPlayerParamHash( ostream & os ) const
{
	if( playerInitialized )
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
	for( int i = mVisualDataQueue.size() - 1; i >= 0; --i )
	{
		// Try to find the object in the currently considered visualHash
		unordered_map<string, VisualData>::const_iterator findIter = mVisualDataQueue[i].find( objName );
		// If it has been found, then estimate the position of the object
		if( findIter != mVisualDataQueue[i].end() )
		{
			int oldTimestamp = findIter->second.timestamp;
			Vector2f oldPosition = findIter->second.absLocation;
			Vector2f oldVelocity = findIter->second.absVelocity;

			if( objName[0] == 'b' )
			{
				unordered_map<string, ServerStruct>::const_iterator ballDecayIter = mServerInfo.find( "ball_decay" );
				if( ballDecayIter != mServerInfo.end() )
				{
					if( oldVelocity[0] == INVALID_FLOAT_VALUE || oldVelocity[1] == INVALID_FLOAT_VALUE )
					{
						// Invalid means it wasn't parsed, just assume velocity was 0 and return the old position
						result = oldPosition;
					}
					else
					{
						result = getFutureBallPos( oldPosition, oldVelocity, currentTimestamp - oldTimestamp, ballDecayIter->second.fValue );
					}
				}
				else
				{
					alwaysAssert();
				}
			}
			else if( objName[0] == 'p' )
			{
				unordered_map<string, ServerStruct>::const_iterator playerDecayIter = mServerInfo.find( "player_decay" );
				if( playerDecayIter != mServerInfo.end() )
				{
					result = getFuturePlayerPos( oldPosition, oldVelocity, currentTimestamp - oldTimestamp, playerDecayIter->second.fValue );
				}
				else
				{
					alwaysAssert();
				}
			}

			break;
		}
	}

	return result;
}

void Player::setTeamName(string teamname)
{
	teamName = teamname;
}

void Player::setPlayerRole( AI_Processing::player_type_t32 role )
{
	playerRole = role;
}

int Player::getUniformNumber() const
{
	return uniformNumber;
}

bool Player::isPlayerInitialized() const
{
	return playerInitialized;
}

void Player::think( queue<string> & commandQueue )
{
	// Use this to retrieve information about the ball
	unordered_map<string, VisualData>::const_iterator ballIter = mVisualDataQueue.back().find( "b" );

	// Determine which team/player possesses the ball 
	determinePossession( commandQueue ); 

	switch( playerRole )
	{
		case PLAYER_TYPE_GOALIE:
		{
//			if( !viewModeSet )
//			{
//				commandQueue.push( Change_View_Cmd( CV_WIDTH_WIDE, CV_QUALITY_HIGH ) );
//				viewModeSet = true;
//			}
			// If the ball is visible, try to intercept it
			if( ballIter != mVisualDataQueue.back().end() )
			{
				// If the ball is on our side of the field, wake up and defend, otherwise
				// just chill and do nothing.
				if( goalieShouldBeActive( side, ballIter->second.absLocation ) )
				{
					Vector2f ballPos = ballIter->second.absLocation;
					Vector2f goaliePos = mSenseBodyDataQueue.back().absLocation;

					vector<VisiblePlayer> teammates;
					vector<VisiblePlayer> opponents;
					if( !mTeammateListQueue.empty() )
					{
						teammates = mTeammateListQueue.back();
					}
					if( !mOpponentListQueue.empty() )
					{
						opponents = mOpponentListQueue.back();
					}
					// Player closest to the ball, not to this client
					VisiblePlayer closestPlayer = getPlayerClosestToLocation( teammates, opponents, ballPos );
					// If the ball is within the catchable area (defined in server.conf, somewhere around 1.2 - 2.0)
					if( ballIter->second.distance <= mServerInfo["catchable_area_l"].fValue )
					{
						commandQueue.push( goalieDoCatchOrKick( side, goaliePos, ballIter->second ) );
					}
					else
					{
						SenseBodyData currSbd = mSenseBodyDataQueue.back();
						string mySide( 1, side );
						string myGoal = "g " + mySide;

						double radius = 10.0f;

						targetPoint = getGoalieTargetPosition( side, mStationaryFlags[myGoal], ballPos, radius );

						if( ( currSbd.absLocation - targetPoint ).magnitude() > 1.0 )
						{
							turnThenDash( currSbd.absLocation, targetPoint, currSbd.absAngle, currSbd.head_angle, ballIter->second.direction, 100, this->dashAfterTurnMode, commandQueue );
						}
						else if( fabs( currSbd.head_angle ) > 0 )
						{
							commandQueue.push( Turn_Cmd( currSbd.head_angle ) );
							commandQueue.push( Turn_Neck_Cmd( -currSbd.head_angle ) );
						}
						else if( fabs( ballIter->second.direction ) > 5 )
						{
							commandQueue.push( Turn_Cmd( ballIter->second.direction / 3 ) );//* ( 1 - 5 / fabs( ballIter->second.direction ) ) ) );
						}
					}
				}
				// If the ball is on the other side of the field, just get back into position
				else
				{
				}

			}
			else
			{
				commandQueue.push( Turn_Cmd( 30 ) );
			}
			break;
		}
		case PLAYER_TYPE_FORWARD:
			//commandQueue.push( Turn_Cmd( 180 ) );
			this->think_forward( commandQueue );
			break;
		case PLAYER_TYPE_MIDFIELDER:
			break;
		case PLAYER_TYPE_DEFENDER:
			break;
		case PLAYER_TYPE_TRAINER:
			break;
	}

}

void Player::think_forward( queue<string> & commandQueue ) //const
{
	
	//-------------------------------------------------------------------------
	// Is calling the hash table every time less efficient than calling 
	// them once at the top then referencing a local variable?
	//-------------------------------------------------------------------------
	
	// Get the most recent visual information
	unordered_map<string, VisualData> visualData = mVisualDataQueue.back();

	// Get most recent senseBody info
	SenseBodyData senseBodyData = mSenseBodyDataQueue.back();

	AuralData auralData = mAuralDataQueue.back();

	//unordered_map<string, AuralData> auralData = mAuralDataQueue.back();

	// get the most recent list of teammates player can see
	vector<VisiblePlayer> teammates;
	if(!mTeammateListQueue.empty())
	{
		teammates = mTeammateListQueue.back();
	}

	// get the most recent list of opponents player can see
	vector<VisiblePlayer> opponents;			
	if(!mOpponentListQueue.empty())
	{
		opponents = mOpponentListQueue.back();
	}

	// useful strings:
	string opponentSide(1, getOpponentSide(side));
	string opponentGoal = "g " + opponentSide;

	bool isTeammateCloser = false;
	bool isSomeoneInYourWay = false;
	int closestPlayer = 0;	

	int multiplier = 1;
	//determine which side you're on
	if(side == 'r')
	{
		multiplier = -1;
	}


	//cout << " SBD: " << senseBodyData.absLocation[0] << ", " << senseBodyData.absLocation[1] << endl;

	//commandQueue.push( Turn_Cmd ( 10 );

	//// if you're in possesion of the ball


	//If you see the ball
	if(visualData.find("b") != visualData.end() ) // || dashAfterTurnMode == true)
	{
		
		// Is there a teammate in your field of view that is closer to the ball than you?		
		if( !mTeammateListQueue.empty() )
		{
			isTeammateCloser = isTeammateCloserBall( mTeammateListQueue.back(), visualData["b"] /*, clientPossessesBall*/ ); 
		}

		//cout << "Ball loc: " << visualData["b"].absLocation[0] << " " << visualData["b"].absLocation[1] << endl;
		//cout << "Ball Dist: " << visualData["b"].distance << endl;
		//cout << "------------------------------\n";

		if (visualData["b"].distance > 20 )
		{
			commandQueue.push( Turn_Neck_Cmd( -1 * senseBodyData.head_angle ) );
			commandQueue.push( Dash_Cmd( 100 ) );
		}
		// You have possession of the ball if you're within 4
		// This is approx. the max distance from the ball when dribbling
		else if( visualData["b"].distance < 4 )
		{
			clientPossessesBall = true;
			commandQueue.push( Turn_Neck_Cmd( -1 * senseBodyData.head_angle ) );

			// If you're within kicking distance
			if(visualData["b"].distance <= 0.7)
			{
				// If you see the goal
				if(visualData.find(opponentGoal) != visualData.end())
				{
					int playerInWay;

					if(!opponents.empty())
					{
						for(unsigned int i = 0; i < opponents.size(); i++)
						{
							if (opponents[i].visualData.distance < 10)
							{
								isSomeoneInYourWay = true;
								playerInWay = i;
							}
						}
					}		

					if (isSomeoneInYourWay)
					{
						int closestTeammate = -1;
						if(!teammates.empty())
						{

							for(unsigned int x = 0; x < teammates.size(); x++)
							{
								if (teammates[x].visualData.distance < teammates[closestTeammate].visualData.distance)
								{
									closestTeammate = x;
								}
							}
						}
						
						if (teammates[closestTeammate].visualData.distance < 20 && closestTeammate != -1)
						{
							commandQueue.push ( Kick_Cmd( 100, teammates[closestTeammate].visualData.direction ) );
						}
						else if ( opponents[playerInWay].visualData.direction < 0)
						{
							targetPoint = Vector2f( opponents[playerInWay].visualData.absLocation[0], opponents[playerInWay].visualData.absLocation[1] + 5 );
							if( ( senseBodyData.absLocation - targetPoint ).magnitude() > 1.0 )
							{
								turnThenDash( senseBodyData.absLocation, targetPoint, senseBodyData.absAngle, senseBodyData.head_angle, visualData["b"].direction, 100, this->dashAfterTurnMode, commandQueue );
							}
						}
						else
						{
							targetPoint = Vector2f( opponents[playerInWay].visualData.absLocation[0], opponents[playerInWay].visualData.absLocation[1] - 5 );
							if( ( senseBodyData.absLocation - targetPoint ).magnitude() > 1.0 )
							{
								turnThenDash( senseBodyData.absLocation, targetPoint, senseBodyData.absAngle, senseBodyData.head_angle, visualData["b"].direction, 100, this->dashAfterTurnMode, commandQueue );
							}
						}
						
					}
					// If you're within the opponent's penalty box
					// absLocation may give odd results if far away from flags
					else if(visualData[opponentGoal].distance < 17)
					//if( checkPlayerBounds(PLAYER_TYPE_GOALIE, visualData["b"].absLocation, getOpponentSide(side)) )
					{
						int goalieInt;
						bool canSeeOpponent = false;
						bool canSeeGoalie = false;

						
						// no opponents to fill queue
						if( !mOpponentListQueue.empty() )
						{	
							canSeeOpponent = true;
							for(unsigned int i = 0; i <= opponents.size() - 1; i++)
							{
								// if you see the goalie, set flag
								if ( opponents[i].isGoalie == true )
								{
									canSeeGoalie = true;
									goalieInt = i;
								}
							}
						}


						if(canSeeGoalie)
						{
							// if the goalie is on the upper side of the goal,
							// kick the ball to the bottom side
							if( opponents[goalieInt].visualData.absLocation[1] >= 0)
							{
								commandQueue.push( Kick_Cmd ( 100, visualData[opponentGoal + "b"].direction - 1.5 ) );
							}
							// if the goalie is on the lower side of the goal,
							// kick the ball to the top side
							else
							{
								commandQueue.push( Kick_Cmd( 100, visualData[opponentGoal + "t"].direction + 1.5 ) );
							}
						}
						else
						{
							// if you can't see the goalie, the goal is open.
							// kick it hard towards the middle
							commandQueue.push( Kick_Cmd( 100, visualData[opponentGoal].direction ) );
						}
					}
					else
					{
						// Dribble the ball
						// This needs some work, player sometimes loses the ball and has to find it again.
						commandQueue.push( Kick_Cmd( 15, visualData[opponentGoal].direction ) );
					}
				}
				// If you don't see the goal
				else
				{
					// Run a bit away from the ball to get a new perspective
					// Maybe you'll see both the goal and the ball.
					commandQueue.push( Dash_Cmd( 35 ) );
					//commandQueue.push( Turn_Cmd( 30 );
				}
			}
			// If you see the ball, but it's at too wide an angle,
			// turn so you're more directly facing it.
			else if ( visualData["b"].direction < -10 ||
						  visualData["b"].direction > 10 )
			{
				// Interesting thing I found...
				// Don't run straight at the ball,
				// Instead, run slightly outside of it (I added '2.0' to the direction)
				// This helps so you're not always in a battle
				// trying to find the ball and the goal at the same time
				commandQueue.push( Turn_Cmd( visualData["b"].direction * 0.5 ) );
			}
			else
			{
				commandQueue.push( Dash_Cmd( 70 ) );
			}
		}
		
		// If you see the ball, but it's at too wide an angle,
		// turn so you're more directly facing it.
		else if ( visualData["b"].direction < -10 ||
				  visualData["b"].direction > 10 )
		{
			// Interesting thing I found...
			// Don't run straight at the ball,
			// Instead, run slightly outside of it (I added '2.0' to the direction)
			// This helps so you're not always in a battle
			// trying to find the ball and the goal at the same time
			commandQueue.push( Turn_Neck_Cmd( -1 * senseBodyData.head_angle ) );
			commandQueue.push( Turn_Cmd( visualData["b"].direction * 0.5) );
		}
		
		// If you're not in possession of the ball
		else if (isTeammateCloser == true)
		{
			unsigned int tooCloseToTeammate = -1;

			// spacing between teammates is about 5
			for (unsigned int i = 0; i < teammates.size(); i++)
			{
				if (teammates[i].visualData.distance < 5)
				{
					tooCloseToTeammate = i;
					break;
				}
			}

			if (tooCloseToTeammate != -1)
			{
				if(teammates[tooCloseToTeammate].visualData.absLocation[0] > senseBodyData.absLocation[0])
				{
					targetPoint = Vector2f( teammates[tooCloseToTeammate].visualData.absLocation[0] - 5, teammates[tooCloseToTeammate].visualData.absLocation[1] + 5);
				}
				else
				{
					targetPoint = Vector2f( teammates[tooCloseToTeammate].visualData.absLocation[0] + 5, teammates[tooCloseToTeammate].visualData.absLocation[1] - 5);
				}

				if( ( senseBodyData.absLocation - targetPoint ).magnitude() > 1.0 )
				{
					turnThenDash( senseBodyData.absLocation, targetPoint, senseBodyData.absAngle, senseBodyData.head_angle, visualData["b"].direction, 50, this->dashAfterTurnMode, commandQueue );
				}
			}
			else if(visualData["b"].distance > 5)
			{

				if (multiplier * (visualData["b"].absLocation[0] - senseBodyData.absLocation[0]) > 0)
				{
					targetPoint = Vector2f( visualData["b"].absLocation[0] + 15*multiplier, senseBodyData.absLocation[1] );
		
					if( ( senseBodyData.absLocation - targetPoint ).magnitude() > 1.0 )
					{
						turnThenDash( senseBodyData.absLocation, targetPoint, senseBodyData.absAngle, senseBodyData.head_angle, visualData["b"].direction, 100, this->dashAfterTurnMode, commandQueue );
					}
				}
				else
				{
					//commandQueue.push( Dash_Cmd( 100 ) );
					//prepare for catch
				}
			}
		}
		// if not, try to get the ball
		else
		{
			commandQueue.push( Dash_Cmd( 100 ) );
		}
	}
	// If you can't see the ball, then turn to find it
	else
	{
		commandQueue.push( Turn_Cmd( 25 ) );
	}														// return whatever command was made
}


string Player::getPlayMode() const
{
	return ( playMode );
}

void Player::determinePossession( queue<string> & commandQueue )
{
	// Determine if ball is visible
	unordered_map<string, VisualData>::const_iterator ballIter = mVisualDataQueue.back().find( "b" );
	if( ballIter != mVisualDataQueue.back().end() )
	{
		// If the this client now possesses the ball, let everyone know
		if( !clientPossessesBall  && doesClientPossessBall( mSenseBodyDataQueue.back().absLocation, ballIter->second.absLocation ) )
		{
			ostringstream temp; 
			temp << "t" << uniformNumber;
			commandQueue.push( Say_Cmd( temp.str() ) );
			clientPossessesBall = true;
			teamPossessesBall = true;
			
	
			cout << "##### clientPossessesBall = " << clientPossessesBall << endl 
				 << "##### teamPossessesBall = " << teamPossessesBall << endl;
		}
	
		// Check if an opponent possesses the ball 
		if( !mOpponentListQueue.empty() )
		{
			if( teamPossessesBall && !clientPossessesBall)
			{
				vector<VisiblePlayer> opponentList = mOpponentListQueue.back();
			
				for( unsigned int i=0; i < opponentList.size(); i++ )
				{
					if( doesClientPossessBall( opponentList[i].visualData.absLocation, ballIter->second.absLocation ) )
					{	
						ostringstream temp; 
						temp << "o" << opponentList[i].uniformNumber;
						commandQueue.push( Say_Cmd( temp.str() ) );
						teamPossessesBall = false;
						clientPossessesBall = false; 
						break;
					}
				}
			}
		}

		// Check if a teammate possesses the ball 
		if( !mTeammateListQueue.empty() )
		{
			if( !teamPossessesBall )
			{
				vector<VisiblePlayer> teammateList = mTeammateListQueue.back();
			
				for( unsigned int i=0; i < teammateList.size(); i++ )
				{
					if( doesClientPossessBall( teammateList[i].visualData.absLocation, ballIter->second.absLocation ) )
					{	
						ostringstream temp; 
						temp << "t" << teammateList[i].uniformNumber;
						commandQueue.push( Say_Cmd( temp.str() ) );
						teamPossessesBall = true;
						break;
					}
				}
			}
		}
	}
	
	//cout << "##### clientPossessesBall = " << clientPossessesBall << endl 
	//	 << "##### teamPossessesBall = " << teamPossessesBall << endl;
}