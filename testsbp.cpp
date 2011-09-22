// testsbp.cpp

#include <string>
#include <iostream>
#include <unordered_map>
#include "sbparser.h"

void printHashData(unordered_map<string, senseBodyData> hashData);

int main()
{
	// example server data
	string inData = "(sense_body 142 (view_mode high normal) (stamina 8000 1 127530) (speed 0.01 0) (head_angle 0) (kick 1) (dash 121) (turn 34) (say 0) (turn_neck 0) (catch 0) (move 1) (change_view 0) (arm (movable 51) (expires 52) (target 53 54) (count 55)) (focus (target none) (count 0)) (tackle (expires 0) (count 0)) (collision none) (foul  (charged 0) (card none)))";

	cout << inData << endl << endl;

	// hash table to hold the results in
	unordered_map<string, senseBodyData> parsedData;

	// parse the server data and return a hash table
	parsedData = parse_sense_body(inData);

	// print out the hash table
	printHashData(parsedData);
}


void printHashData(unordered_map<string, senseBodyData> hashData)
{
	// Here's examples of how to access the data...
	// Because the hash table only accepts one data type,
	// I had to group all the different possible structs into on data type
	//
	// It might just be easier if I return the senseBodyData struct
	// in order for you to directly access the values
	// because that's what you have to do anyways with this method
	//
	// It'll avoid having to use the "hashData[BLAH].BLAH" which
	// seems kind of repetetive.
	
	cout << "timestamp" << ": " << hashData["timestamp"].timestamp << endl;
	
	cout << "view_mode" << ": " << hashData["view_mode"].view_mode.viewQuality << " "
	                            << hashData["view_mode"].view_mode.viewWidth << endl;
	
	cout << "stamina" << ": " << hashData["stamina"].stamina[0] << " "
	                          << hashData["stamina"].stamina[1] << " "
	                          << hashData["stamina"].stamina[2] << endl;
	
	cout << "speed" << ": " << hashData["speed"].speed[0] << " "
	                        << hashData["speed"].speed[1] << endl;
	
	cout << "head_angle" << ": " << hashData["head_angle"].actionCount << endl;
	
	cout << "kick" << ": " << hashData["kick"].actionCount << endl;
	
	cout << "dash" << ": " << hashData["dash"].actionCount << endl;
	
	cout << "turn" << ": " << hashData["turn"].actionCount << endl;
	
	cout << "say" << ": " << hashData["say"].actionCount << endl;
	
	cout << "turn_neck" << ": " << hashData["turn_neck"].actionCount << endl;
	
	cout << "catch" << ": " << hashData["catch"].actionCount << endl;
	
	cout << "move" << ": " << hashData["move"].actionCount << endl;
	
	cout << "change_view" << ": " << hashData["change_view"].actionCount << endl;
	
	cout << "arm" << ": " << hashData["arm"].arm.movable << " "
	                      << hashData["arm"].arm.expires << " "
	                      << hashData["arm"].arm.target[0] << " "
	                      << hashData["arm"].arm.target[1] << " "
	                      << hashData["arm"].arm.count << endl;
	
	cout << "focus" << ": " << hashData["focus"].focus.target << " "
	                        << hashData["focus"].focus.count << endl;
	
	cout << "tackle" << ": " << hashData["tackle"].tackle.expires << " "
	                         << hashData["tackle"].tackle.count << endl;
	
	cout << "collision" << ": " << hashData["collision"].collision << endl;
	
	cout << "foul" << ": " << hashData["foul"].foul.charged << " "
	                       << hashData["foul"].foul.card << endl;
}
