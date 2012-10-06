#pragma once
class Character;
class Maps;
class Settings;
class Player;

class GameMenu
{
public:
	//Menus
	void mainMenu(Settings& currentSettings,Player& p1,Maps& currentLevel,bool gameStarted);
	void mainMenu(Settings& currentSettings);
	void printInstructions();

	//Game Menu Functions
	void save(Settings& currentSettings,Player& p1,Maps& currentLevel);
	void load(Settings& currentSettings,Player& p1,Maps& currentLevel,bool skipToLoad);
	void printMenu(int option,bool gameStarted);
};