/** @file main.cpp
* Main Program Processing
*
* Serves as the main entry point for the program.  Starts and
* initializes each client 
* @author Joseph Wachtel
* @date Oct 19, 2011
*/

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

#define CLIENT_CNT ( 11 )           /* client count                 */
#define TEAM_NAME  ( "team1" )      /* team name                    */

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

/** Main program processing
* @param argc Number of input arguments
* @param *argv[] An array of the inputs arguments
* @pre None
* @post Each client is initialized and started in its own thread while
* the function loops until a keyboard stroke is pressed
* @return Returns a value of 0
*/

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
UDP_client              udp_client[ CLIENT_CNT ];

/*----------------------------------------------------------
Initialization

Todo: Add input parameters for IP, Port, and Teamname
----------------------------------------------------------*/
for( i = 0; i < CLIENT_CNT; i++ )
    {
    tmp_str.str( "" );
    tmp_str << "dbg_log_" << i << ".txt";
    udp_client[ i ].UDP_dbg_log_enbl( tmp_str.str() );
    udp_client[ i ].UDP_open_socket( "192.168.1.3", 6000, TEAM_NAME, i );
    }

while( true ) 
    {
    if( _kbhit() )
        {
        for( i = 0; i < CLIENT_CNT; i++ )
            {
            udp_client[ i ].UDP_close_socket();
            }
        
        break;
        }
    }

return( 0 );

}	/* main() */