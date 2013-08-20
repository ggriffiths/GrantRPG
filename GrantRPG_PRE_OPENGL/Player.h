#pragma once
#include "Character.h"

class Player : public Character
{
	public:
		Player(int new_health,int new_x,int new_y,int new_coins,char new_icon,WORD new_color,bool new_harm,Settings& currentSettings);
		Player(Settings& currentSettings,Maps& currentLevel);
		void attack(Character* enemy,Maps* currentLevel);
};
