// sense_body_parser.h

#ifndef SENSE_BODY_PARSER_H
#define SENSE_BODY_PARSER_H

#include <string>

using namespace std;

struct senseBodyData
{
	
	int timestamp;

	struct viewModeStruct
	{
		string viewQuality, viewWidth;
		/* NOTE: viewQuality should either be "high" or "low"
		         viewWidth should be "narrow", "normal", or "wide" */
	} view_mode;
	
	float stamina[3],
	      speed [2],
	      head_angle,
			kick,
			dash,
			turn,
			say,
			turn_neck,
			catchCount,			// IMPORTANT NOTE: "catch" is a reserved keyword,
									//    so this has been modified to "catchCount"
			move,
			change_view;
	      
	         
	struct armStruct
	{
		float movable, expires, target[2], count;
	} arm;
	
	struct focusStruct
	{
		string target;
		float count;
	} focus;
	
	struct tackleStruct
	{
		float expires, count;
	} tackle;
	
	string collision;
	
	struct foulStruct
	{
		float charged;
		string card;
	} foul;	
};

senseBodyData parse_sense_body(string inData);
#endif

