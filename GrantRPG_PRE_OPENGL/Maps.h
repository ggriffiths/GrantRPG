#pragma once
#include "Character.h"
#include "Player.h"
#include "Npc.h"
#include <string>
#include <windows.h>
using namespace System;

class Items;
class Settings;
class Npc;
class Player;

class Maps
{
public:
	class MapColors
	{
		public:
			MapColors(int world,int level);

			//Map Color Properties
			WORD score;
			WORD wall;
			WORD up;
			WORD down;
			WORD space;
			WORD player;
			WORD door;
			WORD sword;
			WORD npc_noharm;
			WORD npc_harm;
			WORD fence;
			WORD solid_wall;

			// Map Color Functions
			void setColor(WORD color);
	};

	//Ctor for new map
	Maps(int new_level,int new_world,Settings& currentSettings);

	//Ctor for loaded map
	Maps(char new_map[22][42],int new_level,int new_world,Settings& currentSettings,Character* p1);

	//Copy Map
	Maps operator=(Maps& sourceMap);

	// Map Objects
	MapColors cmc;

	//Default Items
	Items const empty;
	Items const sword;
	Items const shield;

	//Map Variables
	int level;
	int world;
	std::string message;
	std::string message2;
	std::string message3;
	std::string old_message;
	char map[22][42];

	//Map Utility Functions
	void pSpaces(int size);
	char getMapValue(int k, int i);

	//Map Handlers
	void handleNpcMovement(Player& p1,Maps* currentLevel);
	void handleNpcAction(Character* p1,Maps* currentLevel);

	//Map Detection
	bool enemyWithinRange(Player& p1,Npc& enemy);

	//Map Actions
	void advanceLevel();
	void advanceWorld();

	//Map Print functions
	void printMap(Player& p1);
	void printStatistics(Player& p1,MapColors& cmc);

	//Npcs
	Npc n1;
	Npc e1;
	Npc e2;

private:
	//Gets map from private
	char storedMapValues(int world,int level,int k, int i);

	//Zones

};