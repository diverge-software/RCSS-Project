#include "Parser.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace Parser;

//using std::cout;
//using std::endl;
using std::string;
using std::vector;
using std::tr1::unordered_map;

string testVisualData = "(see 102 ((p \"Keeler\" 6 goalie) 1 2 3 4 5 6) ((p) 1 2) ((p \"Alpha Males\" 99) 1 2 3 4) ((f c) 19.1 7 0 0) ((f r t) 73.7 -41) ((f r b) 81.5 10) ((f g r b) 71.5 -8) ((g r) 70.1 -14) ((f g r t) 69.4 -20) ((f p r b) 60.3 7) ((f p r c) 53.5 -12) ((f p r t) 54.1 -33) ((f b r 10) 55.1 39) ((f b r 20) 60.3 31) ((f b r 30) 67.4 24) ((f b r 40) 74.4 19) ((f b r 50) 82.3 14) ((f r 0) 75.2 -15) ((f r t 10) 74.4 -22) ((f r t 20) 75.2 -30) ((f r t 30) 77.5 -37) ((f r b 10) 76.7 -7) ((f r b 20) 79.8 0) ((f r b 30) 83.9 6) ((b) 3.7 0 0.148 -0.4) ((l r) 74.4 68))";
string testAuralData = "(hear 120 -78 \"RED CARD!\")";
string testSenseBodyData = "(sense_body 142 (view_mode high normal) (stamina 8000 1 127530) (speed 0.01 0) (head_angle 0) (kick 1) (dash 121) (turn 34) (say 0) (turn_neck 0) (catch 0) (move 1) (change_view 0) (arm (movable 51) (expires 52) (target 53 54) (count 55)) (focus (target none) (count 0)) (tackle (expires 0) (count 0)) (collision none) (foul  (charged 0) (card none)))";
string testPlayerTypeData = "(player_type (id 0)(player_speed_max 1)(stamina_inc_max 45)(player_decay 0.4)(inertia_moment 5)(dash_power_rate 0.006)(player_size 0.3)(kickable_margin 0.7)(kick_rand 0)(extra_stamina 0)(effort_max 1)(effort_min 0.6))";
string testServerData = "(server_param (catch_ban_cycle 5)(clang_advice_win 1)(clang_define_win 1)(clang_del_win 1)(clang_info_win 1(clang_mess_delay 50)(clang_mess_per_cycle 1(clang_meta_win 1)(clang_rule_win 1)(clang_win_size 300(coach_port 6001)(connect_wait 300)(drop_ball_time 0(freeform_send_period 20)(freeform_wait_period 600)(game_log_compression 0)(game_log_version 3)(game_over_wait 100)(goalie_max_moves 2)(half_time -10)(hear_decay 1)(hear_inc 1)(hear_max 1)(keepaway_start -1)(kick_off_wait 100)(max_goal_kicks 3)(olcoach_port 6002)(point_to_ban 5)(point_to_duration 20)(port 6000)(recv_step 10)(say_coach_cnt_max 128)(say_coach_msg_size 128)(say_msg_size 10)(send_step 150)(send_vi_step 100)(sense_body_step 100)(simulator_step 100)(slow_down_factor 1)(start_goal_l 0)(start_goal_r 0)(synch_micro_sleep 1)(synch_offset 60)(tackle_cycles 10)(text_log_compression 0)(game_log_dir \"/home/thoward/data\")(game_log_fixed_name \"rcssserver\")(keepaway_log_dir \"./\")(keepaway_log_fixed_name \"rcssserver\")(landmark_file \"~/.rcssserver-landmark.xml\")(log_date_format \"%YH%M-\")(team_l_start \"\")(team_r_start \"\")(text_log_dir \"/home/thoward/data\")(text_log_fixed_name \"rcssserver\")(coach 0)(coach_w_referee 1)(old_coach_hear 0)(wind_none 0)(wind_random 0)(auto_mode 0)(back_passes 1)(forbid_kick_off_offside 1)(free_kick_faults 1)(fullstate_l 0)(fullstate_r 0)(game_log_dated 1)(game_log_fixed 1)(game_logging 1)(keepaway 0)(keepaway_log_dated 1)(keepaway_log_fixed 0)(keepaway_logging 1)(log_times 0)(profile 0)(proper_goal_kicks 0)(record_messages 0)(send_comms 0)(synch_mode 0)(team_actuator_noise 0)(text_log_dated 1)(text_log_fixed 1)(text_logging 1)(use_offside 1)(verbose 0)(audio_cut_dist 50)(ball_accel_max 2.7)(ball_decay 0.94)(ball_rand 0.05)(ball_size 0.085)(ball_speed_max 2.7)(ball_weight 0.2)(catch_probability 1)(catchable_area_l 2)(catchable_area_w 1)(ckick_margin 1)(control_radius 2)(dash_power_rate 0.006)(effort_dec 0.005)(effort_dec_thr 0.3)(effort_inc 0.01)(effort_inc_thr 0.6)(effort_init 0)(effort_min 0.6)(goal_width 14.02)(inertia_moment 5)(keepaway_length 20)(keepaway_width 20)(kick_power_rate 0.027)(kick_rand 0)(kick_rand_factor_l 1)(kick_rand_factor_r 1)(kickable_margin 0.7)(maxmoment 180)(maxneckang 90)(maxneckmoment 180)(maxpower 100)(minmoment -180)(minneckang -90)(minneckmoment -180)(minpower -100)(offside_active_area_size 2.5))";
string testPlayerParamData = "(player_param (player_types 7)(pt_max 3)(random_seed -1)(subs_max 3)(dash_power_rate_delta_max 0)(dash_power_rate_delta_min 0)(effort_max_delta_factor -0.002)(effort_min_delta_factor -0.002)(extra_stamina_delta_max 100)(extra_stamina_delta_min 0)(inertia_moment_delta_factor 25)(kick_rand_delta_factor 0.5)(kickable_margin_delta_max 0.2)(kickable_margin_delta_min 0)(new_dash_power_rate_delta_max 0.002)(new_dash_power_rate_delta_min 0)(new_stamina_inc_max_delta_factor -10000)(player_decay_delta_max 0.2)(player_decay_delta_min 0)(player_size_delta_factor -100)(player_speed_max_delta_max 0.2)(player_speed_max_delta_min 0)(stamina_inc_max_delta_factor 0))";

void printVisualHash( unordered_map<string, VisualData> visualData );
void printServerHash( unordered_map<string, ServerStruct> serverInfo );
void printPlayerTypes( unordered_map<string, PlayerTypeStruct> playerTypes[] );
void printVisiblePlayers( vector<VisiblePlayer> playerList );
void printAuralStruct( AuralData auralData );
void printSenseBodyData( SenseBodyData sbd );
void printPlayerParamHash( unordered_map<string, PlayerParamStruct> playerType );

int main()
{
	AuralData auralData;
	unordered_map<string, VisualData> visualData;
	vector<VisiblePlayer> playerData;
	SenseBodyData sbd;
	unordered_map<string, PlayerTypeStruct> playerTypes[10];
	unordered_map<string, ServerStruct> serverInfo;
	unordered_map<string, PlayerParamStruct> playerParams;

	parseAuralPacket( testAuralData, auralData );
	parsePlayerTypePacket( testPlayerTypeData, playerTypes );
	parsePlayerParamPacket( testPlayerParamData, playerParams );
	parseSenseBodyPacket( testSenseBodyData, sbd );
	parseServerPacket( testServerData, serverInfo );
	parseVisualPacket( testVisualData, visualData, playerData );

	printVisualHash( visualData );
	printServerHash( serverInfo );
	printPlayerTypes( playerTypes );
	printVisiblePlayers( playerData );
	printAuralStruct( auralData );
	printSenseBodyData( sbd );
	printPlayerParamHash( playerParams );

	return 0;
}

void printVisualHash( unordered_map<string, VisualData> visualData )
{
	cout << "############################################" << endl;
	cout << "##            Visual Information          ##" << endl;
	cout << "############################################" << endl;
	for( unordered_map<string, VisualData>::const_iterator it = visualData.begin(); it != visualData.end(); ++it )
	{
		cout << "[\"" << it->first << "\", " << it->second.distance << ", " << it->second.direction;
		if( it->second.distanceChange != INVALID_FLOAT_VALUE )
		{
			cout << ", " << it->second.distanceChange << ", " << it->second.directionChange << "]" << endl;
		}
		else
		{
			cout << "]" << endl;
		}
	}
}

void printServerHash( unordered_map<string, ServerStruct> serverInfo )
{
	cout << "############################################" << endl;
	cout << "##            Server Information          ##" << endl;
	cout << "############################################" << endl;
	for( unordered_map<string, ServerStruct>::const_iterator it = serverInfo.begin(); it != serverInfo.end(); ++it )
	{
		if( it->second.sValue == "" )
		{
			cout << "[\"" << it->first << "\", " << it->second.fValue << "]" << endl;
		}
		else
		{
			cout << "[\"" << it->first << "\", " << it->second.sValue << "]" << endl;
		}
	}
}

void printPlayerTypes( unordered_map<string, PlayerTypeStruct> playerTypes[] )
{
	cout << "############################################" << endl;
	cout << "##         Player Type Information        ##" << endl;
	cout << "############################################" << endl;
	for( int i = 0; i < 10; i++ )
	{
		for( unordered_map<string, PlayerTypeStruct>::const_iterator it = playerTypes[i].begin(); it != playerTypes[i].end(); ++it )
		{
			cout << "[\"" << it->first << "\", " << it->second.fValue << "]" << endl;
		}
	}
}

void printPlayerParamHash( unordered_map<string, PlayerParamStruct> playerTypes )
{
	cout << "############################################" << endl;
	cout << "##         Player Param Information        ##" << endl;
	cout << "############################################" << endl;
	for( int i = 0; i < 10; i++ )
	{
		for( unordered_map<string, PlayerParamStruct>::const_iterator it = playerTypes.begin(); it != playerTypes.end(); ++it )
		{
			cout << "[\"" << it->first << "\", " << it->second.fValue << "]" << endl;
		}
	}
}

void printVisiblePlayers( vector<VisiblePlayer> playerList )
{
	cout << "############################################" << endl;
	cout << "##            Player Information          ##" << endl;
	cout << "############################################" << endl;
	for( int i = 0; i < playerList.size(); i++ )
	{
		cout << "[p, teamName = ";
		if( playerList[i].teamName != INVALID_TEAM_NAME )
		{
			cout << playerList[i].teamName;
		}
		else
		{
			cout << "UNKNOWN";
		}
		if( playerList[i].uniformNumber != INVALID_UNIFORM_NUMBER )
		{
			cout << ", uniform = " << playerList[i].uniformNumber;
		}
		else
		{
			cout << ", uniform = UNKNOWN";
		}
		if( playerList[i].isGoalie )
		{
			cout << ", is goalie";
		}

		cout << ", " << playerList[i].visualData.distance << ", " << playerList[i].visualData.direction;

		if( playerList[i].visualData.directionChange != INVALID_FLOAT_VALUE )
		{
			cout << ", " << playerList[i].visualData.distanceChange << ", " << playerList[i].visualData.directionChange;
			if( playerList[i].bodyDirection != INVALID_FLOAT_VALUE )
			{
				cout << ", " << playerList[i].bodyDirection << ", " << playerList[i].headDirection;
			}
			cout << "]" << endl;
		}
		else
		{
			cout << "]" << endl;
		}
	}
}

void printAuralStruct( AuralData auralData )
{
	cout << "############################################" << endl;
	cout << "##            Aural Information           ##" << endl;
	cout << "############################################" << endl;

	cout << "Time: " << auralData.timestamp << endl;

	if( auralData.sender != INVALID_SENDER_NAME )
	{
		cout << "Sender: " << auralData.sender << endl;
	}
	else
	{
		cout << "Src Direction: " << auralData.direction << endl;
	}

	cout << "Message: " << auralData.message << endl;
}

void printSenseBodyData( SenseBodyData sbd )
{
	cout << "############################################" << endl;
	cout << "##          Sense Body Information        ##" << endl;
	cout << "############################################" << endl;
	cout << "timestamp" << ": " << sbd.timestamp << endl;
	
	cout << "view_mode" << ": " << sbd.view_mode.viewQuality << " "
	                            << sbd.view_mode.viewWidth << endl;
	
	cout << "stamina" << ": " << sbd.stamina[0] << " "
	                          << sbd.stamina[1] << " "
	                          << sbd.stamina[2] << endl;
	
	cout << "speed" << ": " << sbd.speed[0] << " "
	                        << sbd.speed[1] << endl;
	
	cout << "head_angle" << ": " << sbd.head_angle << endl;
	
	cout << "kick" << ": " << sbd.kick << endl;
	
	cout << "dash" << ": " << sbd.dash << endl;
	
	cout << "turn" << ": " << sbd.turn << endl;
	
	cout << "say" << ": " << sbd.say << endl;
	
	cout << "turn_neck" << ": " << sbd.turn_neck << endl;
	
	cout << "catch" << ": " << sbd.catchCount << endl;
	
	cout << "move" << ": " << sbd.move << endl;
	
	cout << "change_view" << ": " << sbd.change_view << endl;
	
	cout << "arm" << ": " << sbd.arm.movable << " "
	                      << sbd.arm.expires << " "
	                      << sbd.arm.target[0] << " "
	                      << sbd.arm.target[1] << " "
	                      << sbd.arm.count << endl;
	
	cout << "focus" << ": " << sbd.focus.target << " "
	                        << sbd.focus.count << endl;
	
	cout << "tackle" << ": " << sbd.tackle.expires << " "
	                         << sbd.tackle.count << endl;
	
	cout << "collision" << ": " << sbd.collision << endl;
	
	cout << "foul" << ": " << sbd.foul.charged << " "
	                       << sbd.foul.card << endl;
}
