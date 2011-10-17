/*********************************************************************
*
*   MODULE NAME:
*       main.cpp - Main Program Processing
*
*   DESCRIPTION:
*       Performs Main Program Processing
*
*---------------------------------------------------------------------
* $Id: main.cpp, v1.4, 2011-10-11 17:25:00Z, Joseph Wachtel$
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
#define TEAM_NAME "team1"
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
ostringstream           tmp_str;
UDP_client              udp_client[ 11 ];

/*----------------------------------------------------------
Initialization

Todo: Add input parameters for IP, Port, and Teamname
----------------------------------------------------------*/
for( i = 0; i < 11; i++ )
    {
    tmp_str.str( "" );
    tmp_str << "dbg_log_" << i << ".txt";
    udp_client[ i ].UDP_dbg_log_enbl( tmp_str.str() );
    udp_client[ i ].UDP_open_socket( /*"192.168.1.3"*/"10.0.1.52", 6000, TEAM_NAME, i );
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