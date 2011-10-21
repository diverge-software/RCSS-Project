#include "Brain.h"
#include "Debug.hpp"
#include <iostream>

using namespace std;
using std::tr1::unordered_map;

bool Brain::isTeammateOpenForPass(VisiblePlayer teammate, vector<VisiblePlayer> opponents)
{
	//determine if an opponent is in position to intercept
	for(unsigned int i=0; i < opponents.size(); i++)
	{
		if( (opponents[i].visualData.distance <= teammate.visualData.distance)
			 && (opponents[i].visualData.direction <= 5)
			 && (opponents[i].visualData.direction >= -5) )
		{		
			return false;
		}
	}

	return true;	
}

Vector2f Brain::getFuturePlayerPos(Vector2f cPos, Vector2f cVec, double tInterval)
{
	return cPos + cVec * tInterval;
}

Vector2f Brain::getFutureBallPos(Vector2f cPos, Vector2f cVec, double tInterval, double ballDecay)
{
	return cPos + cVec * pow(ballDecay, tInterval-1); 
}