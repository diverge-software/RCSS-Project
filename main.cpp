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
#define TEAM_NAME  ( "diverge" )    /* team name                    */

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
boolean                 error;      /* error occurred               */
int                     i;          /* index                        */
string                  input;      /* input                        */
ostringstream           tmp_str;
UDP_client              udp_client[ CLIENT_CNT ];
string                  server_ip;
unsigned int            server_port;

cout << "Welcome to the Diverge Software RoboCup Soccer Client" << endl << endl; 

cout << "Enter RoboCup Soccer Server information [xx.xx.xx.xx:port]: ";
cin >> input;

server_ip = input.substr( 0, input.find( ":" ) );
server_port = atoi( input.substr( input.find( ":" ) + 1, input.length() ).c_str() );

//server_ip = "192.168.1.3";
//server_port = 6000;

/*----------------------------------------------------------
Initialization

Todo: Add input parameters for IP, Port, and Teamname
----------------------------------------------------------*/
error = FALSE;

for( i = 0; i < CLIENT_CNT; i++ )
    {
    tmp_str.str( "" );
    tmp_str << "dbg_log_" << i << ".txt";
    udp_client[ i ].UDP_dbg_log_enbl( tmp_str.str() );

    if( !udp_client[ i ].UDP_open_socket( server_ip, server_port, TEAM_NAME ) )
        {
        cout << endl << "Error: Client unable to connect to the RoboCup Soccer Server" << endl;
        error = TRUE;
        break;
        }
    }

if( !error )
    {
    cout << endl << "Debug Interface:" << endl << endl;
    cout << "sp: Stop drills" << endl;
    cout << "st: Start drills" << endl;
    cout << "mv: Move player [mv uniform x y]" << endl;
    cout << "qt: Quit" << endl << endl;

    while( true ) 
        {
        cout << "Command: ";
        cin >> input;

        /*--------------------------------------------------
        Quit command
        --------------------------------------------------*/
        if( input.compare( "qt" ) == 0 )
            {
            for( i = 0; i < CLIENT_CNT; i++ )
                {
                udp_client[ i ].UDP_close_socket();
                }

            return( 0 );
            }
        }
    }

cout << endl;
system( "PAUSE" );

return( 0 );

}	/* main() */