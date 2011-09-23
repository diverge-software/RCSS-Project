/*********************************************************************
*
*   MODULE NAME:
*       main.cpp - Main Program Processing
*
*   DESCRIPTION:
*       Performs Main Program Processing
*
*---------------------------------------------------------------------
* $Id: main.cpp, v1.0, 2011-09-23 17:25:00Z, Joseph Wachtel$
* $NoKeywords$
*********************************************************************/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <iostream>
#include <conio.h>

#include "udp_client.hpp"

using namespace std;

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           PROJECT INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/


/*********************************************************************
*
*   PROCEDURE NAME:
*       main - Main Program Processing
*
*   DESCRIPTION:
*       Main program processing
*
*********************************************************************/

int main							/* main program processing		*/
	(
	int					argc,		/* argument c					*/
	char				*argv[]		/* argument v					*/
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
UDP_client              udp_client_0;
UDP_client              udp_client_1;
UDP_client              udp_client_2;
UDP_client              udp_client_3;
UDP_client              udp_client_4;
UDP_client              udp_client_5;
UDP_client              udp_client_6;
UDP_client              udp_client_7;
UDP_client              udp_client_8;
UDP_client              udp_client_9;
UDP_client              udp_client_10;

string                  tst;

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
udp_client_0.UDP_open_socket( "Team1" );
udp_client_1.UDP_open_socket( "Team1" );
udp_client_2.UDP_open_socket( "Team1" );
udp_client_3.UDP_open_socket( "Team1" );
udp_client_4.UDP_open_socket( "Team1" );
udp_client_5.UDP_open_socket( "Team1" );
udp_client_6.UDP_open_socket( "Team1" );
udp_client_7.UDP_open_socket( "Team1" );
udp_client_8.UDP_open_socket( "Team1" );
udp_client_9.UDP_open_socket( "Team1" );
udp_client_10.UDP_open_socket( "Team1" );

while( true )
    {
    if( _kbhit() )
        {
        break;
        }

    if( udp_client_0.UDP_retreive( &tst ) )
        {
        // This is where we "Think" and parse data
        //cout << tst << endl;
        }
    }

return( 0 );

}	/* main() */