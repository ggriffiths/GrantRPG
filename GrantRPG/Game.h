#pragma once
#include "Character.h"
#include "Maps.h"
#include "Settings.h"
#include "Player.h"
#include "Npc.h"

class Game
{
public:
	class Sound
	{
	public:
		void getCoin();
		void getItem();
		void activate();
		void advanceLevel();
		void collision();
		void moveSound();
		void attackSound();
	};
	Game(Settings& currentSettings,Player& p1,Maps& currentLevel,bool isNewGame);

	//Primary Game Objects
	Player p1;
	Maps currentLevel;
	Settings currentSettings;
	Sound gameSounds;

	//Game Variables
	bool newGame;
	int gameSpeed;

	//Main Game Control Functions
	void runGame();
	void update(int key);

	//Detects keys and handles accordingly
	int handleKeyPress();
	void handleMiscDetection();	
	
	//Detect if keys are pressed
	bool anyKeyPressed();
	int keyPressed();
};