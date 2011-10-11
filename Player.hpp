#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Parser.hpp"
#include "Vector2f.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <unordered_map>
#include <queue>

using namespace std;
using namespace Parser;

#define NUM_PLAYER_TYPES 17
#define MAX_QUEUE_SIZE 10

class Player
{
	public:
		Player();
		// parseBuffer should eventually be a private member
		bool parseBuffer( const string buffer );

		// Printing functions for parsed data
		void printNewestVisualHash( ostream & os );
		void printNewestVisiblePlayersList( ostream & os );
		void printNewestAuralStruct( ostream & os );
		void printNewestSenseBodyStruct( ostream & os );
		void printServerHash( ostream & os );
		void printPlayerTypesHash( ostream & os );
		void printPlayerParamHash( ostream & os );

	private:
		// Sensory data passed from the server
		queue<AuralData>                          mAuralDataQueue;
		queue<SenseBodyData>                      mSenseBodyDataQueue;
		queue<vector<VisiblePlayer>>              mPlayerListQueue;
		queue<unordered_map<string, VisualData>>  mVisualDataQueue;

		// Initialization data passed from the server
		int uniformNumber;
		char side;    // Which side you're playing from, left or right
		unordered_map<string, PlayerTypeStruct>   mPlayerTypes[NUM_PLAYER_TYPES];
		unordered_map<string, ServerStruct>       mServerInfo;
		unordered_map<string, PlayerParamStruct>  mPlayerParams;
		unordered_map<string, Vector2f>           mStationaryFlags;
};
	
#endif