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
int                     i;
UDP_client              udp_client[ 11 ];

/*----------------------------------------------------------
Initialization

Todo: Add input parameters for IP, Port, and Teamname
----------------------------------------------------------*/
for( i = 0; i < 1; i++ )
    {
    udp_client[ i ].UDP_open_socket( "192.168.1.3", 6000, "Team1", i );
    }

while( true ) 
    {
    if( _kbhit() )
        {
        break;
        }
    }

return( 0 );

}	/* main() */