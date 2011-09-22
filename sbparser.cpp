#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <vector>


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
	
	float stamina[3], speed [2], actionCount;
	/* NOTE: actionCount is used for...
	           - head_angle
	           - kick
	           - dash
	           - turn
	           - say
	           - turn_neck
	           - catch
	           - move
	           - change_view */
	           
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
} sbd;


unordered_map<string, senseBodyData> parse_sense_body(string inData)
{
	// create an empty hash table to store the keys and their values
	unordered_map<string, senseBodyData> hashData;
	
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
		hashData["timestamp"] = sbd;

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
			
			hashData[key] = sbd;
			key = strtok(NULL, " ()");
		}
   }
   
	return hashData;
}

