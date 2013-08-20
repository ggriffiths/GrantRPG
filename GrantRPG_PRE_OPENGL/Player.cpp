//--Player.cpp--//
#include "Player.h"

//--Project Headers--//
#include "Game.h"
#include "GameMenu.h"
#include "Items.h"
#include "Character.h"
#include "Maps.h"
#include "Settings.h"

//--Language Headers--//
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <ctime>

//--Namespaces--//
using namespace std;
using namespace System;

//Player constructor!
Player::Player(int new_health,int new_x,int new_y,int new_coins,char new_icon,WORD new_color,bool new_harm,Settings& currentSettings) 
: 
Character()
{
	character_type=2;
	name="Player";
	alive=true;
	Inventory inv(false);
	health=new_health;
	x=new_x;
	y=new_y;
	coins=new_coins;
	icon=currentSettings.playerIcon;
	color=new_color;
	harm=new_harm;
	attackDmg=2;
}

// Player Ctor (default)
Player::Player(Settings& currentSettings,Maps& currentLevel)
{
	character_type=2;
	name="Player";
	alive=true;
	Inventory inv(false);
	health=10;
	for(unsigned int k=0;k<22;k++)
	{
		for(unsigned int i=0;i<42;i++)
		{
			currentLevel.map[k][i]=currentLevel.getMapValue(k,i);
			if(currentLevel.map[k][i]==2)
			{
				setLocation(k,i);
			}
		}
	}
	coins=0;
	icon=currentSettings.playerIcon;
	color='e';
	harm=false;
	attackDmg=4;
}

//Base character attacks target character.
void Player::attack(Character* enemy,Maps* currentLevel)
{
	Game::Sound gameSounds;
	int randProb=0;
	srand((unsigned int)time(0));
	randProb=(1+rand()%10);
	if(randProb>skill)
	{
		gameSounds.attackSound();
		currentLevel->message=("You attacked " + enemy->name + "!");
		enemy->health=enemy->health-attackDmg;
	}
	if(enemy->health<1)
	{
		enemy->alive=false;
		currentLevel->message="You Killed a " + enemy->name + "!";
		currentLevel->map[enemy->y][enemy->x]=8;
		enemy->x=0;
		enemy->y=0;
	}
}
