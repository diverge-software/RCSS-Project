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
#include "Debug.hpp"

using namespace std;

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define CLIENT_CNT ( 12 )           /* client count                 */
#define TEAM_NAME  ( "diverge" )    /* team name                    */
#define TRNR_UNUM  ( 12 )           /* trainer uniform number       */

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

static vector< string > split_str   /* split string into tokens     */
    ( 
    string              in_str      /* input string                 */
    );


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
int                     defender;   /* defender count               */
boolean                 error;      /* error occurred               */
int                     forward;    /* forward count                */
int                     goalie;     /* goalie count                 */
int                     i;          /* index                        */
string                  input;      /* input                        */
int                     midfielder; /* midfielder count             */
int                     player_cnt; /* player count                 */
string                  pt_str;     /* player type string           */
string                  server_ip;  /* server IP                    */
unsigned int            server_port;/* server port                  */
int                     trainer;    /* trainer count                */
boolean                 trainer_enbld;
                                    /* trainer enabled              */
ostringstream           tmp_str;    /* temporary string             */
UDP_client *            udp_client[ CLIENT_CNT ];
                                    /* UDP client pointer array     */
UDP_client *            udp_client_ptr;
                                    /* UDP client pointer           */
int                     uniform_num;/* uniform number               */
vector< string >        vec_str;    /* vector string                */

string teamName;
if( argc == 1 )
{
	teamName = TEAM_NAME;
}
else if( argc == 2 )
{
	teamName = argv[1];
}
else
{
	alwaysAssert();
}

/*----------------------------------------------------------
Get server information for user
----------------------------------------------------------*/
cout << "Welcome to the Diverge Software RoboCup Soccer Client" << endl << endl; 

cout << "Enter RoboCup Soccer Server information [xx.xx.xx.xx:port]: ";
cin >> input;

//input = "192.168.1.105:6000";
server_ip = input.substr( 0, input.find( ":" ) );
server_port = atoi( input.substr( input.find( ":" ) + 1, input.length() ).c_str() );

/*----------------------------------------------------------
Enable/Disable the offline trainer
----------------------------------------------------------*/
cout << "Enable the offline trainer [y/n]: ";
cin >> input;
cout << endl;

if( (input.compare( "y" ) == 0) || (input.compare( "Y" ) == 0) )
    {
    trainer_enbld = TRUE;

    /*------------------------------------------------------
    Specify goalie enabled
    ------------------------------------------------------*/
    cout << "Enable the goalie [y/n]: ";
    cin >> input;

    if( (input.compare( "y" ) == 0) || (input.compare( "Y" ) == 0) )
        {
        goalie = 1;
        }
    else
        {
        goalie = 0;
        }

    /*------------------------------------------------------
    Specify number of forwards to enable
    ------------------------------------------------------*/
    cout << "Number of forwards to enable [0-3]: ";
    cin >> input;

    while( atoi( input.c_str() ) > 3 )
        {
        cout << "Invalid input" << endl;
        cin >> input;
        }

    forward = atoi( input.c_str() );

    /*------------------------------------------------------
    Specify number of defenders to enable
    ------------------------------------------------------*/
    cout << "Number of defenders to enable [0-3]: ";
    cin >> input;

    while( atoi( input.c_str() ) > 3 )
        {
        cout << "Invalid input" << endl;
        cin >> input;
        }

    defender = atoi( input.c_str() );

    /*------------------------------------------------------
    Specify number of midfielders to enable
    ------------------------------------------------------*/
    cout << "Number of midfielders to activate [0-4]: ";
    cin >> input;

    while( atoi( input.c_str() ) > 4 )
        {
        cout << "Invalid input" << endl;
        cin >> input;
        }

    midfielder = atoi( input.c_str() );

    cout << endl;
    trainer = 1;
    player_cnt = goalie + defender + forward + midfielder + trainer;
    }
else
    {
    trainer_enbld = FALSE;

    goalie     = 1;
    defender   = 3;
    midfielder = 4;
    forward    = 3;

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
    pt_str.clear();

    /*------------------------------------------------------
    Enable debug logging for client 
    ------------------------------------------------------*/
    tmp_str.str( "" );
    tmp_str << "dbg_log_" << i << ".txt";
    udp_client_ptr->UDP_dbg_log_enbl( tmp_str.str() );

    /*------------------------------------------------------
    Assign player goalie type 
    ------------------------------------------------------*/
    if( goalie-- > 0 )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port, teamName, PLAYER_TYPE_GOALIE );
        pt_str = "Goalie";
        }

    /*------------------------------------------------------
    Assign player defender type 
    ------------------------------------------------------*/
    else if( defender-- > 0 )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port, teamName, PLAYER_TYPE_DEFENDER );
        pt_str = "Defender";
        }

    /*------------------------------------------------------
    Assign player forward type 
    ------------------------------------------------------*/
    else if( forward-- > 0 )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port, teamName, PLAYER_TYPE_FORWARD );
        pt_str = "Forward";
        }

    /*------------------------------------------------------
    Assign player midfielder type 
    ------------------------------------------------------*/
    else if( midfielder-- > 0 )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port, teamName, PLAYER_TYPE_MIDFIELDER );
        pt_str = "Midfielder";
        }

    /*------------------------------------------------------
    Assign trainer type
    ------------------------------------------------------*/
    else if( trainer-- > 0 )
        {
        uniform_num = udp_client_ptr->UDP_open_socket( server_ip, server_port + 1, teamName, PLAYER_TYPE_TRAINER );
        pt_str = "Trainer";
        }

    if( pt_str.compare( "" ) != 0 )
        {
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
        cout << "mv: Move player/ball [mv [b/p] [uniform] x y]" << endl;
        }

    cout << "rw: Send raw command to server" << endl;
    cout << "qt: Quit" << endl << endl;
    getline( cin, input );

    while( true ) 
        {
        cout << "Command: ";
        getline( cin, input );
        vec_str = split_str( input );

        /*--------------------------------------------------
        Quit command
        --------------------------------------------------*/
        if( vec_str[ 0 ].compare( "qt" ) == 0 )
            {
            if( trainer_enbld )
                {
                for( i = 0; i < player_cnt - 1; i++ )
                    {
                    udp_client[ i ]->UDP_close_socket();
                    }

                udp_client[ 11 ]->UDP_close_socket();
                }
            else
                {
                for( i = 0; i < player_cnt; i++ )
                    {
                    udp_client[ i ]->UDP_close_socket();
                    }
                }

            return( 0 );
            }

        /*--------------------------------------------------
        Stop drill command
        --------------------------------------------------*/
        else if( ( vec_str[ 0 ].compare( "sp" ) == 0 )
              && ( trainer_enbld                     ) )
            {
            udp_client[ TRNR_UNUM - 1 ]->UDP_send( "(change_mode time_over)" );
            }

        /*--------------------------------------------------
        Start drill command
        --------------------------------------------------*/
        else if( ( vec_str[ 0 ].compare( "st" ) == 0 )
              && ( trainer_enbld                     ) )
            {
            udp_client[ TRNR_UNUM - 1 ]->UDP_send( "(change_mode play_on)" );
            }

        /*--------------------------------------------------
        Move command
        --------------------------------------------------*/
        else if( ( vec_str[ 0 ].find( "mv" ) == 0 )
              && ( trainer_enbld                  ) )
            {
            if( vec_str[ 1 ].compare( "b" ) == 0 )
                {
                if( vec_str.size() < 4 )
                    {
                    cout << "Invalid Format" << endl;
                    }
                else
                    {
                    tmp_str.str( "" );
                    tmp_str << "(move (ball) " << vec_str[ 2 ] << " " << vec_str[ 3 ] << ")";

                    udp_client[ TRNR_UNUM - 1 ]->UDP_send( tmp_str.str() );
                    }
                }
            else if( vec_str[ 1 ].compare( "p" ) == 0 )
                {
                if( vec_str.size() < 5 )
                    {
                    cout << "Invalid Format" << endl;
                    }
                else
                    {
                    tmp_str.str( "" );
                    tmp_str << "(move (player " << teamName << " " << vec_str[ 2 ] << ") " << vec_str[ 3 ] << " " << vec_str[ 4 ] << ")";

                    udp_client[ TRNR_UNUM - 1 ]->UDP_send( tmp_str.str() );
                    }
                }
            else
                {
                cout << "Invalid Format: mv [b/p] [uniform] x y" << endl;
                }           
            }

        /*--------------------------------------------------
        Raw server command
        --------------------------------------------------*/
        else if( vec_str[ 0 ].find( "rw" ) == 0 )
            {
            input.erase( 0, 3 );

            udp_client[ TRNR_UNUM - 1 ]->UDP_send( input );
            }

        /*--------------------------------------------------
        Unknown command
        --------------------------------------------------*/
        else
            {
            cout << "Unknown Command" << endl;
            }

        Sleep( 200 );
        }
    }

cout << endl;
system( "PAUSE" );

return( 0 );

}	/* main() */


/** Split String Into Tokens
* @param None
* @pre None
* @post Outputs to standard out the Winsock API error message
* @return Returns A Queue Of Strings
*/

static vector< string > split_str   /* split string into tokens     */
    ( 
    string              in_str      /* input string                 */
    )
{
/*----------------------------------------------------------
Local Variables
----------------------------------------------------------*/
int                     position;   /* position                     */
vector< string >        ret_val;    /* return value                 */

/*----------------------------------------------------------
Initialization
----------------------------------------------------------*/
position = in_str.find( " " );

/*----------------------------------------------------------
Split the string into tokens
----------------------------------------------------------*/
while( position != -1 )
    {
    ret_val.push_back( in_str.substr( 0, position ) );
    in_str.erase( 0, position + 1 );

    position = in_str.find( " " );
    }

ret_val.push_back( in_str.substr( 0, in_str.length() ) );

return( ret_val );

}   /* split_str() */