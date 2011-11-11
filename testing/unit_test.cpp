/** @file unit_test.hpp
* Contains several unit tests for AI Processing functions
* @author Joseph Wachtel
* @date Nov 08, 2011
*/

#include "ai_processing.hpp"
#include "WinUnit.h"

using namespace AI_Processing;

/*----------------------------------------------------------
Module Test: Move_Cmd()

Tested the move command both equvialency and boundary 
conditions.  This verifies the function will assert if the
wrong value is sent, and that the string is being formatted
correctly for the server
----------------------------------------------------------*/
BEGIN_TEST( move_cmd_test )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    boolean             assertion;
    string              ret_val;

    /*------------------------------------------------------
    Equivalency Case
    ------------------------------------------------------*/
    ret_val = AI_Processing::Move_Cmd( 10, 10 );
    WIN_ASSERT_TRUE( ret_val == "(move 10 10)" );

    /*------------------------------------------------------
    Equivalency Case
    ------------------------------------------------------*/
    ret_val = AI_Processing::Move_Cmd( -10, -10 );
    WIN_ASSERT_TRUE( ret_val == "(move -10 -10)" );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    try
        {
        assertion = FALSE;

        ret_val = AI_Processing::Move_Cmd( -52.501, 10 );
        }
    catch( int err_code )
        {
        assertion = TRUE;
        WIN_ASSERT_TRUE( err_code == -1 );
        }

    WIN_ASSERT_TRUE( assertion == TRUE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    try
        {
        assertion = FALSE;

        ret_val = AI_Processing::Move_Cmd( 52.501, 10 );
        }
    catch( int err_code )
        {
        assertion = TRUE;
        WIN_ASSERT_TRUE( err_code == -1 );
        }

    WIN_ASSERT_TRUE( assertion == TRUE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    try
        {
        assertion = FALSE;

        ret_val = AI_Processing::Move_Cmd( 0, -34.001 );
        }
    catch( int err_code )
        {
        assertion = TRUE;
        WIN_ASSERT_TRUE( err_code == -1 );
        }

    WIN_ASSERT_TRUE( assertion == TRUE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    try
        {
        assertion = FALSE;

        ret_val = AI_Processing::Move_Cmd( 0, 34.001 );
        }
    catch( int err_code )
        {
        assertion = TRUE;
        WIN_ASSERT_TRUE( err_code == -1 );
        }

    WIN_ASSERT_TRUE( assertion == TRUE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    try
        {
        assertion = FALSE;

        ret_val = AI_Processing::Move_Cmd( 0, 34 );
        }
    catch( int err_code )
        {
        assertion = TRUE;
        WIN_ASSERT_TRUE( err_code == -1 );
        }

    WIN_ASSERT_TRUE( assertion == FALSE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    try
        {
        assertion = FALSE;

        ret_val = AI_Processing::Move_Cmd( 0, -34 );
        }
    catch( int err_code )
        {
        assertion = TRUE;
        WIN_ASSERT_TRUE( err_code == -1 );
        }

    WIN_ASSERT_TRUE( assertion == FALSE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    try
        {
        assertion = FALSE;

        ret_val = AI_Processing::Move_Cmd( -52.5, 0 );
        }
    catch( int err_code )
        {
        assertion = TRUE;
        WIN_ASSERT_TRUE( err_code == -1 );
        }

    WIN_ASSERT_TRUE( assertion == FALSE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    try
        {
        assertion = FALSE;

        ret_val = AI_Processing::Move_Cmd( 52.5, 0 );
        }
    catch( int err_code )
        {
        assertion = TRUE;
        WIN_ASSERT_TRUE( err_code == -1 );
        }

    WIN_ASSERT_TRUE( assertion == FALSE );
    }
END_TEST

/*----------------------------------------------------------
Function Test: checkPlayerBounds()

Tested for the goalie player type both left and right sides
of the field
----------------------------------------------------------*/
BEGIN_TEST( check_player_bounds_test )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    boolean             ret_val;
    Vector2f *          abs_location;
    char                side;

    /*------------------------------------------------------
    Equivalency Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( -35, 0 );
    side = 'l';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == TRUE );

    /*------------------------------------------------------
    Equivalency Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( -15, 0 );
    side = 'l';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == FALSE );

    /*------------------------------------------------------
    Equivalency Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( -35, 35 );
    side = 'l';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == FALSE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( -32.001, -21.999 );
    side = 'l';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == TRUE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( -49.999, 21.999 );
    side = 'l';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == TRUE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( -50, -22 );
    side = 'l';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == FALSE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( -32, 22 );
    side = 'l';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == FALSE );

    /*------------------------------------------------------
    Equivalency Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( 35, 0 );
    side = 'r';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == TRUE );

    /*------------------------------------------------------
    Equivalency Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( 15, 0 );
    side = 'r';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == FALSE );

    /*------------------------------------------------------
    Equivalency Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( 35, 35 );
    side = 'r';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == FALSE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( 32.001, -21.999 );
    side = 'r';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == TRUE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( 49.999, 21.999 );
    side = 'r';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == TRUE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( 50, -22 );
    side = 'r';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == FALSE );

    /*------------------------------------------------------
    Boundary Case
    ------------------------------------------------------*/
    abs_location = new Vector2f( 32, 22 );
    side = 'r';
    ret_val = AI_Processing::checkPlayerBounds( PLAYER_TYPE_GOALIE, *abs_location, side );
    WIN_ASSERT_TRUE( ret_val == FALSE );
    }
END_TEST


/*----------------------------------------------------------
Function Test: getAbsAngleToLocation()

Tested the absolute position angle calculation
----------------------------------------------------------*/
BEGIN_TEST( get_abs_angle_to_location_test )
    {
    /*------------------------------------------------------
    Local Variables
    ------------------------------------------------------*/
    double              ret_val;
    Vector2f *          location;
    Vector2f *          player_pos;

    /*------------------------------------------------------
    Case 1
    ------------------------------------------------------*/
    location = new Vector2f( 10, 10 );
    player_pos = new Vector2f( 10, 10 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val == -90 );

    /*------------------------------------------------------
    Case 2
    ------------------------------------------------------*/
    location = new Vector2f( 10, 10 );
    player_pos = new Vector2f( 10, 15 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val == -90 );

    /*------------------------------------------------------
    Case 3
    ------------------------------------------------------*/
    location = new Vector2f( 10, 15 );
    player_pos = new Vector2f( 10, 10 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val == 90 );

    /*------------------------------------------------------
    Case 4
    ------------------------------------------------------*/
    location = new Vector2f( 15, 10 );
    player_pos = new Vector2f( 10, 10 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val == 0 );

    /*------------------------------------------------------
    Case 5
    ------------------------------------------------------*/
    location = new Vector2f( 10, 10 );
    player_pos = new Vector2f( 15, 10 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val == 180 );

    /*------------------------------------------------------
    Case 6
    ------------------------------------------------------*/
    location = new Vector2f( 15, 15 );
    player_pos = new Vector2f( 10, 10 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val > 0.999890 && ret_val < 0.999899 );

    /*------------------------------------------------------
    Case 7
    ------------------------------------------------------*/
    location = new Vector2f( -15, -10 );
    player_pos = new Vector2f( 10, 12 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val < -179.120068 && ret_val > -179.1200699 );

    /*------------------------------------------------------
    Case 8
    ------------------------------------------------------*/
    location = new Vector2f( -15, 12 );
    player_pos = new Vector2f( 10, 10 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val > 179.9200000 && ret_val < 179.9200001 );

    /*------------------------------------------------------
    Case 9
    ------------------------------------------------------*/
    location = new Vector2f( 15, 10 );
    player_pos = new Vector2f( 10, 12 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val < -0.399993 && ret_val > -0.399994 );

    /*------------------------------------------------------
    Case 10
    ------------------------------------------------------*/
    location = new Vector2f( 15, 10 );
    player_pos = new Vector2f( 10, 12 );
    ret_val = AI_Processing::getAbsAngleToLocation( *player_pos, *location );
    WIN_ASSERT_TRUE( ret_val < -0.399993 && ret_val > -0.399994 );
    }
END_TEST
