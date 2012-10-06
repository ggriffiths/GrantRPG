#pragma once
class Character;
class Maps;
class Player;

#include <string>

class Settings
{
	public:
		Settings(bool loadFromFile,int s1,int s2,bool s3,bool s4);
		//Settings menus
		void settingsMenu(Settings& currentSettings,Player& p1,Maps& currentLevel);

		//Settings print functions
		void printSettings(std::string setting[],int value[],int option);
		void printSpacer(int spaces);

		//Setting Properties
		int playerIcon;
		int moveSpeed;
		bool musOn;
		bool saOn;
};

