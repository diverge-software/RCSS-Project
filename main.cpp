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

#define CLIENT_CNT   ( 12 )         /* client count                 */
#define TEAM_NAME    ( "diverge" )  /* team name                    */

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
int                     player_cnt; /* player count                 */
string                  pt_str;     /* player type string           */
string                  server_ip;  /* server IP                    */
unsigned int            server_port;/* server port                  */
boolean                 trainer_enbld;
                                    /* trainer enabled              */
ostringstream           tmp_str;    /* temporary string             */
UDP_client *            udp_client[ CLIENT_CNT ];
                                    /* UDP client pointer array     */
UDP_client *            udp_client_ptr;
                                    /* UDP client pointer           */
int                     uniform_num;/* uniform number               */

/*----------------------------------------------------------
Get server information for user
----------------------------------------------------------*/
cout << "Welcome to the Diverge Software RoboCup Soccer Client" << endl << endl; 

cout << "Enter RoboCup Soccer Server information [xx.xx.xx.xx:port]: ";
cin >> input;

server_ip = input.substr( 0, input.find( ":" ) );
server_port = atoi( input.substr( input.find( ":" ) + 1, input.length() ).c_str() );

/*----------------------------------------------------------
Enable/Disable the offline trainer
----------------------------------------------------------*/
cout << "Enable the offline trainer [y/n]: ";
cin >> input;
cout << endl;

if( input.compare( "y" ) == 0 )
    {
    trainer_enbld = TRUE;
    player_cnt = CLIENT_CNT;
    }
else
    {
    trainer_enbld = FALSE;
    player_cnt = CLIENT_CNT - 1;
    }

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
error = FALSE;

/*----------------------------------------------------------
Assign player types for each soccer client
----------------------------------------------------------*/
for( i = 0; i < player_cnt; i++ )
    {   
    udp_client_ptr = new UDP_client;

    /*------------------------------------------------------
    Enable debug logging for client 
    ------------------------------------------------------*/
    tmp_str.str( "" );
    tmp_str << "dbg_log_" << i << ".txt";
    udp_client_ptr->UDP_dbg_log_enbl( tmp_str.str() );

    /*------------------------------------------------------
    Assign player goalie type 
    ------------------------------------------------------*/
    if( i == 0 )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port, TEAM_NAME, PLAYER_TYPE_GOALIE );
        pt_str = "Goalie";
        }

    /*------------------------------------------------------
    Assign player defender type 
    ------------------------------------------------------*/
    else if( ( i > 0 )
          && ( i < 4 ) )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port, TEAM_NAME, PLAYER_TYPE_DEFENDER );
        pt_str = "Defender";
        }

    /*------------------------------------------------------
    Assign player forward type 
    ------------------------------------------------------*/
    else if( ( i >= 4 )
          && ( i < 8  ) )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port, TEAM_NAME, PLAYER_TYPE_FORWARD );
        pt_str = "Forward";
        }

    /*------------------------------------------------------
    Assign player midfielder type 
    ------------------------------------------------------*/
    else if( ( i >= 8 )
          && ( i < 11 ) )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port, TEAM_NAME, PLAYER_TYPE_MIDFIELDER );
        pt_str = "Midfielder";
        }

    /*------------------------------------------------------
    Assign trainer type
    ------------------------------------------------------*/
    else if( trainer_enbld )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port + 1, TEAM_NAME, PLAYER_TYPE_TRAINER );
        pt_str = "Trainer";
        }

    if( uniform_num == -1 )
        {
        cout << endl << "Error: Client unable to connect to the RoboCup Soccer Server" << endl;
        error = TRUE;

        break;
        }
    else
        {
        udp_client[ uniform_num - 1 ] = udp_client_ptr;
        cout << "Uniform Number: " << uniform_num << " Player Type: " << pt_str << endl;
        }
    }

/*----------------------------------------------------------
Display debug interface and wait for an input
----------------------------------------------------------*/
if( !error )
    {
    cout << endl << "User Interface:" << endl << endl;

    if( trainer_enbld )
        {
        cout << "sp: Stop drills" << endl;
        cout << "st: Start drills" << endl;
        cout << "mv: Move player [mv uniform x y]" << endl;
        }

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
            for( i = 0; i < player_cnt; i++ )
                {
                udp_client[ i ]->UDP_close_socket();
                }

            return( 0 );
            }
        }
    }

cout << endl;
system( "PAUSE" );

return( 0 );

}	/* main() */