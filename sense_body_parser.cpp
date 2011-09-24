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
	char *key, *val;
	
	key = strtok(str, " ()");
	
	// make sure sense_body server info is being parsed
	if (strcmp(key, "sense_body") == 0)
	{
		// the first token that comes after "sense_body" is a time stamp
		// get it and store it in the hash table
		val = strtok(NULL, " ()");
		sbd.timestamp = atoi(val);

		//get the next key
		key = strtok(NULL, " ()");
		
		// as long as you haven't reached the end of the string of server data
		while(key !=NULL && val != NULL)
		{
			// if the key is any of the actionCount types
			// get the value and store it in the hash table with the corresponding key
			if(strcmp(key, "head_angle") == 0 ||
			   strcmp(key, "kick") == 0 ||
			   strcmp(key, "dash") == 0 ||
			   strcmp(key, "turn") == 0 ||
			   strcmp(key, "say") == 0 ||
			   strcmp(key, "turn_neck") == 0 ||
			   strcmp(key, "catch") == 0 ||
			   strcmp(key, "move") == 0 ||
			   strcmp(key, "change_view") == 0)
			{
				val = strtok(NULL, " ()");
				sbd.actionCount = atof(val);
			}
			else if(strcmp(key, "view_mode") == 0)
			{
				sbd.view_mode.viewQuality = strtok(NULL, " ()");
				sbd.view_mode.viewWidth = strtok(NULL, " ()");
			}
			else if(strcmp(key, "stamina") == 0)
			{
				for(int i = 0; i < 3; i++)
				{
					val = strtok(NULL, " ()");
					sbd.stamina[i] = atof(val);
				}
			}
			else if (strcmp(key, "arm") == 0)
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
			else if (strcmp(key, "speed") == 0)
			{
				for(int i = 0; i < 2 ; i++)
				{
					val = strtok(NULL, " ()");
					sbd.speed[i] = atof(val);
				}
			}
			else if (strcmp(key, "tackle") == 0)
			{
				strtok(NULL, " ()");
				sbd.tackle.expires = atof(strtok(NULL, " ()"));
				strtok(NULL, " ()");
				sbd.tackle.count = atof(strtok(NULL, " ()"));
			}
			else if (strcmp(key, "focus") == 0)
			{
				val = strtok(NULL, " ()");
				sbd.focus.target = strtok(NULL, " ()");
				val = strtok(NULL, " ()");
				sbd.focus.count = atof(strtok(NULL, " ()"));
			}
			else if (strcmp(key, "collision") == 0)
			{
				sbd.collision = strtok(NULL, " ()");
			}
			else if (strcmp(key,"foul") == 0)
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

			key = strtok(NULL, " ()");
		}
   }
   
	return sbd;
}

