#ifndef DEMO_HPP
#define DEMO_HPP

#include "udp_client.hpp"
#include <string>

using namespace std;

string makeThemMove(unsigned int playerNum, char* buffer);

string movePlayersOntoField(unsigned int playerNum);

string makePlayersRunAroundOnField(unsigned int playerNum, char* buffer);

#endif 