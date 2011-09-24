// testsbp.cpp

#include <string>
#include <iostream>
#include <unordered_map>
#include "sense_body_parser.h"

void printParsedData(senseBodyData hashData);

int main()
{
	// example server data
	string inData = "(sense_body 142 (view_mode high normal) (stamina 8000 1 127530) (speed 0.01 0) (head_angle 0) (kick 1) (dash 121) (turn 34) (say 0) (turn_neck 0) (catch 0) (move 1) (change_view 0) (arm (movable 51) (expires 52) (target 53 54) (count 55)) (focus (target none) (count 0)) (tackle (expires 0) (count 0)) (collision none) (foul  (charged 0) (card none)))";

	cout << inData << endl << endl;

	// Create senseBodyData instance to hold the parsed data
	senseBodyData sbd;
	
	// parse the server data and return the struct
	sbd = parse_sense_body(inData);

	// print out the struct table
	printParsedData(sbd);
}


void printParsedData(senseBodyData sbd)
{	
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
