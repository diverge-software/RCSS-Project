#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Parser.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <unordered_map>

using namespace std;
using namespace Parser;

#define NUM_PLAYER_TYPES 17

class Player
{
	public:
		Player();
		// parseBuffer should eventually be a private member
		bool parseBuffer( const string buffer );

		// Printing functions for parsed data
		void printVisualHash( ostream & os );
		void printServerHash( ostream & os );
		void printPlayerTypesHash( ostream & os );
		void printVisiblePlayersList( ostream & os );
		void printAuralStruct( ostream & os );
		void printSenseBodyStruct( ostream & os );
		void printPlayerParamHash( ostream & os );

	private:
		// Sensory data passed from the server
		AuralData auralData;
		SenseBodyData senseBodyData;
		vector<VisiblePlayer> playerList;
		unordered_map<string, VisualData> visualData;

		// Initialization data passed from the server
		int uniformNumber;
		char side;    // Which side you're playing from, left or right
		unordered_map<string, PlayerTypeStruct> playerTypes[NUM_PLAYER_TYPES];
		unordered_map<string, ServerStruct> serverInfo;
		unordered_map<string, PlayerParamStruct> playerParams;
};
	
#endif