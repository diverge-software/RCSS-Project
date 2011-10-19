#include "udp_client.hpp"
#include "demo.hpp"

string movePlayersOntoField(unsigned int playerNum)
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
string                  ret_val;    /* return value                 */

switch( playerNum )
    {
    case 0:
        ret_val = "(move -10 10)";
        break;

    case 1:
        ret_val = "(move -5 0)";
        break;

    case 2:
        ret_val = "(move -10 0)";
        break;

    case 3:
        ret_val = "(move -15 0)";
        break;

    case 4:
        ret_val = "(move -20 0)";
        break;

    case 5:
        ret_val = "(move -5 5)";
        break;

    case 6:
        ret_val = "(move -5 -5)";
        break;

    case 7:
        ret_val = "(move -5 10)";
        break;

    case 8:
        ret_val = "(move -5 -10)";
        break;

    case 9:
        ret_val = "(move -5 15)";
        break;

    case 10:
        ret_val = "(move -5 -15)";
        break;

    default:
        ret_val = "";
        break;
    }

return( ret_val );

}

string makePlayersRunAroundOnField(unsigned int playerNum)
{

return( "(dash 30)" );

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