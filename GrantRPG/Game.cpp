//Game.cpp
#include "Game.h"

//--Project Headers--//
#include "GameMenu.h"
#include "Maps.h"
#include "Character.h"
#include "Items.h"

//--Language Headers--//
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <math.h>
#include <time.h>

//--Namespaces--//
using namespace std;
using namespace System;

//--Global Variables--//
enum keys{UPRIGHT=1,UPLEFT,DOWNRIGHT,DOWNLEFT,UP,DOWN,RIGHT,LEFT,E,ENTER,C,TAB,MAX_NUM_KEYS};

//Constructor...
Game::Game(Settings& currentSettings,Player& p1,Maps& currentLevel,bool isNewGame) 
: 
currentSettings(true,0,0,false,false),
currentLevel(1,1,currentSettings),
p1(currentSettings,currentLevel)
{
	newGame=isNewGame;
	gameSpeed=(11-currentSettings.moveSpeed);
}

// Holds the main loop...
void Game::runGame()
{
	//Initialize objects
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
	int key=0;
	
	// Print initial map
	currentLevel.printMap(p1);

	// Main Game 
	bool paused=false;
	while(!paused)
	{
		//Start Timer
	    clock_t step_time;
		step_time=500+clock();

		//hold loop until valid keypress
		while(((!anyKeyPressed() && step_time>clock())) || paused);
		key=keyPressed();
		
		//Update The Game
		update(key);

		//Print the Game
		currentLevel.printMap(p1);
	}	
}

// Updates the game based on a key pressed
void Game::update(int key)
{
	//Checks for events and handles them accordingly
	p1.handlePlayerMovement(key,&currentLevel);
	p1.handlePlayerAction(key,&currentLevel);
	currentLevel.handleNpcMovement(p1,&currentLevel);
	currentLevel.handleNpcAction(&p1,&currentLevel);	
	handleMiscDetection();
}

// Handles Misc stuff
void Game::handleMiscDetection()
{
	bool tab_ctrl=false;
	tab_ctrl=false;
	while(keyPressed()==TAB)
	{
		system("CLS");
		p1.showInv();
		tab_ctrl=true;
	}
	if(tab_ctrl) currentLevel.printMap(p1);
	if(GetAsyncKeyState(VK_ESCAPE))
	{
		GameMenu menuObj;
		menuObj.mainMenu(currentSettings,p1,currentLevel,true);			
	} 
	// Update map when press enter
	else if(keyPressed()==ENTER) // ENTER
	{
		currentLevel.printMap(p1);
	}
}

// Checks all keys and returns true if no keys are pressed.
bool Game::anyKeyPressed()
{
	for(int i=1;i<=MAX_NUM_KEYS;i++)
	{
		if(keyPressed()==i) return true;
	}
	return false;
}

// Checks if given key has been pressed
int Game::keyPressed()
{
	if((GetAsyncKeyState(VK_UP))&&(GetAsyncKeyState(VK_RIGHT)) || (GetAsyncKeyState(0x57))&&(GetAsyncKeyState(0x44))) return UPRIGHT;
	else if((GetAsyncKeyState(VK_UP))&&(GetAsyncKeyState(VK_LEFT)) || (GetAsyncKeyState(0x57))&&(GetAsyncKeyState(0x41))) return UPLEFT;
	else if((GetAsyncKeyState(VK_DOWN))&&(GetAsyncKeyState(VK_RIGHT)) || (GetAsyncKeyState(0x53))&&(GetAsyncKeyState(0x44))) return DOWNRIGHT;
	else if((GetAsyncKeyState(VK_DOWN))&&(GetAsyncKeyState(VK_LEFT))|| (GetAsyncKeyState(0x53))&&(GetAsyncKeyState(0x41))) return DOWNLEFT;
	else if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57)) return UP;
	else if(GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) return DOWN;
	else if(GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44)) return RIGHT;
	else if(GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(0x41)) return LEFT;
	else if(GetAsyncKeyState(0x45)) return E;
	else if(GetAsyncKeyState(VK_RETURN)) return ENTER;
	else if(GetAsyncKeyState(0x43)) return C;
	else if(GetAsyncKeyState(VK_TAB)) return TAB;
	else return 0;
}

// Sound for obtaining a coin
void Game::Sound::getCoin()
{
	Beep(300,50);
	Beep(200,50);
	Beep(500,75);
}

// Sound for bumping into a wall
void Game::Sound::collision()
{
	Beep(150,50);
	Beep(100,50);
}

// Sound for obtaining an item
void Game::Sound::getItem()
{
	Beep(400,50);
	Beep(300,50);
	Beep(500,50);
	Beep(450,50);
	Beep(500,50);
	Beep(600,100);
}

// Sound for activating
void Game::Sound::activate()
{
	Beep(500,50);
	Beep(400,50);
	Beep(600,50);
	Beep(550,50);
	Beep(600,50);
	Beep(700,100);
}

// Sound for advancing a level
void Game::Sound::advanceLevel()
{
	Beep(500,100);
	Beep(400,50);
	Beep(600,200);
}

// Sound for movement
void Game::Sound::moveSound()
{
	Beep(200,5);
}

// Sound for attacking
void Game::Sound::attackSound()
{
	Beep(600,50);
}