// sense_body_parser.cpp

#include <iostream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include "sense_body_parser.h"

using namespace std;


senseBodyData parse_sense_body(string inData)
{
	// Create senseBodyData struct to hold the data from the server
	senseBodyData sbd;
	
	// Convert the server data from string to c_string
	char str[inData.size()+1];
	strcpy (str, inData.c_str());

	// begin separating the string into tokens
	char *val;
	
	val = strtok(str, " ()");
	
	// make sure sense_body server info is being parsed
	if (strcmp(val, "sense_body") == 0)
	{
		// the first token that comes after "sense_body" is a time stamp
		// get it and store it in the hash table
		val = strtok(NULL, " ()");
		sbd.timestamp = atoi(val);

		//get the next val
		val = strtok(NULL, " ()");
		
		// as long as you haven't reached the end of the string of server data
		while(val !=NULL && val != NULL)
		{
			// if the val is any of the actionCount types
			// get the value and store it in the hash table with the corresponding val
			if(strcmp(val, "head_angle") == 0)
			{
				sbd.head_angle = atof(strtok(NULL, " ()"));
			}
			else if(strcmp(val, "kick") == 0)
			{
				sbd.kick = atof(strtok(NULL, " ()"));
			}
			else if(strcmp(val, "dash") == 0)
			{
				sbd.dash = atof(strtok(NULL, " ()"));
			}
			else if(strcmp(val, "turn") == 0)
			{
				sbd.turn = atof(strtok(NULL, " ()"));
			}
			else if(strcmp(val, "say") == 0)
			{
				sbd.say = atof(strtok(NULL, " ()"));
			}
			else if(strcmp(val, "turn_neck") == 0)
			{
				sbd.turn_neck = atof(strtok(NULL, " ()"));
			}
			else if(strcmp(val, "catch") == 0)
			{
				sbd.catchCount = atof(strtok(NULL, " ()"));
			}
			else if(strcmp(val, "move") == 0)
			{
				sbd.move = atof(strtok(NULL, " ()"));
			}
			else if(strcmp(val, "change_view") == 0)
			{
				sbd.change_view = atof(strtok(NULL, " ()"));
			}
			else if(strcmp(val, "view_mode") == 0)
			{
				sbd.view_mode.viewQuality = strtok(NULL, " ()");
				sbd.view_mode.viewWidth = strtok(NULL, " ()");
			}
			else if(strcmp(val, "stamina") == 0)
			{
				for(int i = 0; i < 3; i++)
				{
					sbd.stamina[i] = atof(strtok(NULL, " ()"));
				}
			}
			else if (strcmp(val, "arm") == 0)
			{
				val = strtok(NULL, " ()");
				sbd.arm.movable = atof(strtok(NULL, " ()"));
				val = strtok(NULL, " ()");
				sbd.arm.expires = atof(strtok(NULL, " ()"));
				val = strtok(NULL, " ()");
				sbd.arm.target[0] = atof(strtok(NULL, " ()"));
				sbd.arm.target[1] = atof(strtok(NULL, " ()"));
				val = strtok(NULL, " ()");
				sbd.arm.count = atof(strtok(NULL, " ()"));
			}
			else if (strcmp(val, "speed") == 0)
			{
				for(int i = 0; i < 2 ; i++)
				{
					sbd.speed[i] = atof(strtok(NULL, " ()"));
				}
			}
			else if (strcmp(val, "tackle") == 0)
			{
				strtok(NULL, " ()");
				sbd.tackle.expires = atof(strtok(NULL, " ()"));
				strtok(NULL, " ()");
				sbd.tackle.count = atof(strtok(NULL, " ()"));
			}
			else if (strcmp(val, "focus") == 0)
			{
				val = strtok(NULL, " ()");
				sbd.focus.target = strtok(NULL, " ()");
				val = strtok(NULL, " ()");
				sbd.focus.count = atof(strtok(NULL, " ()"));
			}
			else if (strcmp(val, "collision") == 0)
			{
				sbd.collision = strtok(NULL, " ()");
			}
			else if (strcmp(val,"foul") == 0)
			{
				strtok(NULL, " ()");
				sbd.foul.charged = atof(strtok(NULL, " ()"));
				strtok(NULL, " ()");
				sbd.foul.card = strtok(NULL, " ()");
			}
			else
			{
				cout << "this key is not recognized.\n";
			}

			val = strtok(NULL, " ()");
		}
   }
   
	return sbd;
}

