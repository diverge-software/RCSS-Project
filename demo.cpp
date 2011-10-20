#include "udp_client.hpp"
#include "demo.hpp"
#include "Player.hpp"
#include <stdio.h>

/// I'm not exactly sure how to integrate the player class into this.


string movePlayersOntoField(unsigned int playerNum )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
string                  ret_val;    /* return value                 */

	switch( playerNum )
	{
	case 0:
		ret_val = "(move -0.3 0)";
		break;

	case 1:
		ret_val = "(move -10 0)";
		break;

	case 2:
		ret_val = "(move -10 -10)";
		break;

	case 3:
		ret_val = "(move -10 10)";
		break;

	case 4:
		ret_val = "(move -10 -20)";
		break;

	case 5:
		ret_val = "(move -10 20)";
		break;

	case 6:
		ret_val = "(move -20 10)";
		break;

	case 7:
		ret_val = "(move -20 -10)";
		break;

	case 8:
		ret_val = "(move -20 -20)";
		break;

	case 9:
		ret_val = "(move -20 20)";
		break;

	case 10:
		ret_val = "(move -20 0)";
		break;

	default:
		ret_val = "";
		break;
	}

	return( ret_val );

}

string makePlayersRunAroundOnField(unsigned int playerNum, char* buffer)
{
	
	/* The program should run ok up til here.
	   There's something wrong with how the players are moving together.
	   Most likely it's the kicking or reading the buffer.
	   
	   I just wanted to get these changes up before any major changes were made.
	*/
	float bDist, bDir;

	if(strstr(buffer, "(b)"))
	{
		sscanf_s(buffer,"(b) %f %f", &bDist, &bDir);
	}
	else
	{
		bDist = 1000;
		bDir = 1000;
	}


	if(bDist < 0.7)
	{
		return ("(kick 10 0)");
	}
	else
	{
		return( "(dash 5)" );
	}

}


string makeThemMove(unsigned int playerNum, char* buffer)
{
	string tx_str="";

	if(strstr(buffer, "(see 0") != NULL)
	{
		tx_str = movePlayersOntoField( playerNum );
	}
	else
	{
		tx_str = makePlayersRunAroundOnField( playerNum, buffer );
	}

	if( !tx_str.empty() )
	{
		return ( tx_str );
	}
}