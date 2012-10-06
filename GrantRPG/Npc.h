#pragma once
#include "Character.h"

class Npc : public Character
{
public:
	//Constructor
	Npc(int new_npc_type,Settings& currentSettings,Maps& currentLevel);

	//AI functions
	void attack(Character* enemy,Maps* currentLevel);
	void wander(int chance,Maps& currentLevel);
	void goToCharacter(Character& c1,Maps& currentLevel);
	void goToCoords(int coord_x,int coord_y,Maps& currentLevel);
	int findBestStep(int spot_x,int spot_y);
	double findDistance(int spot_x,int spot_y,int nX,int nY);
	int* pathClear(Character& c1,Maps* currentLevel);
	int* navigateToOpening(Character& c1,Maps* currentLevel,int*coords);
	

	//test
	int destX;
	int destY;

	bool playerSpotted;
};