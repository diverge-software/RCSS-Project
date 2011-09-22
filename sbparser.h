// sbparser.hpp

#ifndef SENSE_BODY_PARSER_H
#define SENSE_BODY_PARSER_H

#include <string>
#include <unordered_map>
#include "sbparser.cpp"
using namespace std;

unordered_map<string, senseBodyData> parse_sense_body(string inData);
//void printHashData(unordered_map<string, senseBodyData> hashData);



#endif

