/** @file Brain.h
* Module used for parsing S-expressions from the server.
* @author Keeler Russell, Jared Mar, Corbin Charpentier
* @date Oct 15, 2011
*/
#ifndef BRAIN_H
#define BRAIN_H

/* Contributions by Jared Mar, Keeler Russell, and Corbin Charpentier, all grouped into one big module */

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <string.h>
#include <cstdlib>
#include <cmath>
#include <deque>

#include "Vector2f.hpp"
#include "Parser.hpp"

using namespace Parser;

using std::string;
using std::vector;
using std::tr1::unordered_map;

namespace Brain
{
	/** Determines for client-player if a particular teammate can be passed to or if an opponent is in range to intercept
	 * @param teammate A struct containing all visible information about teammate in question
	 * @param opponnents A vector containing structs of an opponents visual information
	 * @pre None
	 * @post Client-Player knows who it can or cannot pass to a particular player
	 * @return True if teammate is passable (open), False if teammate is not
	 */
	bool isTeammateOpenForPass(VisiblePlayer teammate, vector<VisiblePlayer> opponents);

	/** Determines future position of a player given its current position and velocity 
     * @param cPos
     * @param cVec
	 * @pre Non
	 * @post Client-Player knows future of position a player
	 * @return returns future position as a vector
	 */
	Vector2f getFuturePlayerPos(Vector2f cPos, Vector2f cVec, double tInterval, double ballDecay);
	
	/** Determines future position of a player given its current position and velocity 
     * @param cPos
     * @param cVec
	 * @pre Non
	 * @post Client-Player knows future of position a player
	 * @return returns future position as a vector
	 */
	Vector2f getFutureBallPos(Vector2f cPos, Vector2f cVec, double tInterval, double ballDecal);	
}

#endif