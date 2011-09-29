#include "Player.hpp"
#include "Parser.hpp"
#include "Debug.hpp"

#include <iostream>
using std::cout;
using std::endl;

// Initialize internal structs to invalid values
Player::Player()
{
	auralData.timestamp = -1;
	auralData.sender = INVALID_SENDER_NAME;
	auralData.direction = INVALID_FLOAT_VALUE;
	auralData.message = INVALID_STRING_VALUE;

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
}

// Decide the buffer type and pass to respective parsing function
bool Player::parseBuffer(const string buffer)
{
	if( isBufferComplete( buffer ) )
	{
		// Things parsed more often are put near top
		if( !buffer.compare( 0, 4, "(see" ) )
		{
			parseVisualPacket( buffer, this->visualData, this->playerList );
		}
		else if( !buffer.compare( 0, 11, "(sense_body" ) )
		{
			parseSenseBodyPacket( buffer, this->senseBodyData );
		}
		else if( !buffer.compare( 0, 5, "(hear" ) )
		{
			parseAuralPacket( buffer, this->auralData );
		}
		else if( !buffer.compare( 0, 5, "(init" ) )
		{
			parseInitPacket( buffer, this->uniformNumber, this->side );
		}
		else if( !buffer.compare( 0, 13, "(server_param" ) )
		{
			parseServerPacket( buffer, this->serverInfo );
		}
		else if( !buffer.compare( 0, 12, "(player_type" ) ) 
		{
			parsePlayerTypePacket( buffer, this->playerTypes );
		}
		else if( !buffer.compare( 0, 13, "(player_param" ) )
		{
			parsePlayerParamPacket( buffer, this->playerParams );
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

void Player::printVisualHash( ostream & os )
{
	os << "############################################" << endl;
	os << "##            Visual Information          ##" << endl;
	os << "############################################" << endl;
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

void Player::printServerHash( ostream & os )
{
	os << "############################################" << endl;
	os << "##            Server Information          ##" << endl;
	os << "############################################" << endl;
	for( unordered_map<string, ServerStruct>::const_iterator it = serverInfo.begin(); it != serverInfo.end(); ++it )
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

void Player::printPlayerTypesHash( ostream & os )
{
	os << "############################################" << endl;
	os << "##         Player Type Information        ##" << endl;
	os << "############################################" << endl;
	for( int i = 0; i < NUM_PLAYER_TYPES; i++ )
	{
		os << "PLAYER TYPE #" << i << endl;
		for( unordered_map<string, PlayerTypeStruct>::const_iterator it = playerTypes[i].begin(); it != playerTypes[i].end(); ++it )
		{
			os << "[\"" << it->first << "\", " << it->second.fValue << "]" << endl;
		}
	}
}

void Player::printVisiblePlayersList( ostream & os )
{
	os << "############################################" << endl;
	os << "##            Player Information          ##" << endl;
	os << "############################################" << endl;
	for( unsigned int i = 0; i < playerList.size(); i++ )
	{
		os << "[p, teamName = ";
		if( playerList[i].teamName != INVALID_TEAM_NAME )
		{
			os << playerList[i].teamName;
		}
		else
		{
			os << "UNKNOWN";
		}
		if( playerList[i].uniformNumber != INVALID_UNIFORM_NUMBER )
		{
			os << ", uniform = " << playerList[i].uniformNumber;
		}
		else
		{
			os << ", uniform = UNKNOWN";
		}
		if( playerList[i].isGoalie )
		{
			os << ", is goalie";
		}

		os << ", " << playerList[i].visualData.distance << ", " << playerList[i].visualData.direction;

		if( playerList[i].visualData.directionChange != INVALID_FLOAT_VALUE )
		{
			os << ", " << playerList[i].visualData.distanceChange << ", " << playerList[i].visualData.directionChange;
			if( playerList[i].bodyDirection != INVALID_FLOAT_VALUE )
			{
				os << ", " << playerList[i].bodyDirection << ", " << playerList[i].headDirection;
			}
			os << "]" << endl;
		}
		else
		{
			os << "]" << endl;
		}
	}
}

void Player::printAuralStruct( ostream & os )
{
	os << "############################################" << endl;
	os << "##            Aural Information           ##" << endl;
	os << "############################################" << endl;

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

void Player::printSenseBodyStruct( ostream & os )
{
	os << "############################################" << endl;
	os << "##          Sense Body Information        ##" << endl;
	os << "############################################" << endl;
	os << "timestamp" << ": " << senseBodyData.timestamp << endl;
	
	os << "view_mode" << ": " << senseBodyData.view_mode.viewQuality << " "
	                            << senseBodyData.view_mode.viewWidth << endl;
	
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

void Player::printPlayerParamHash( ostream & os )
{
	os << "############################################" << endl;
	os << "##         Player Param Information        ##" << endl;
	os << "############################################" << endl;
	for( int i = 0; i < 10; i++ )
	{
		for( unordered_map<string, PlayerParamStruct>::const_iterator it = playerParams.begin(); it != playerParams.end(); ++it )
		{
			os << "[\"" << it->first << "\", " << it->second.fValue << "]" << endl;
		}
	}
}
