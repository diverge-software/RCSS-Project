#include "udp_client.hpp"
#include "demo.hpp"
//#include <string>

string movePlayersOntoField(unsigned int playerNum)
{

    switch (playerNum)
        {
        case 0:
            return "(move -10 10)";
            break;
        case 1:
            return "(move -5 0)";
            break;
        case 2:
            return "(move -10 0)";
            break;
        case 3:
            return "(move -15 0)";
            break;
        case 4:
            return "(move -20 0)";
            break;
        case 5:
            return "(move -5 5)";
            break;
        case 6:
            return "(move -5 -5)";
            break;
        case 7:
            return "(move -5 10)";
            break;
        case 8:
            return "(move -5 -10)";
            break;
        case 9:
            return "(move -5 15)";
            break;
        case 10:
            return "(move -5 -15)";
            break;
        }

/*
	if(playerNum == 4)
	{
		return "(move 10 10)";
	}
	else
	{
		return "don't do anything";
	}
*/
}

string makePlayersRunAroundOnField(unsigned int playerNum)
{

	return "(dash 30)";

/*
	if(playerNum == 4)
	{        
		return "(dash 20)";
    }
	else
	{
		return "don't do anything";
	}
*/
}

/*
string makeThemMove(unsigned int playerNum)
{
    string serverString;

    return ( makePlayersRunAroundOnField(playerNum) );
}*/ 