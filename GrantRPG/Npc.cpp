//--Npc.cpp--//
#include "Npc.h"

//--Project Headers--//
#include "Game.h"
#include "GameMenu.h"
#include "Items.h"
#include "Character.h"
#include "Maps.h"
#include "Settings.h"
#include "Npc.h"
#include "Player.h"

//--Language Headers--//
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <ctime>

//--Namespaces--//
using namespace std;
using namespace System;

//--Global Variables--//
enum keys{UPRIGHT=1,UPLEFT,DOWNRIGHT,DOWNLEFT,UP,DOWN,RIGHT,LEFT,E,ENTER,C,TAB,MAX_NUM_KEYS};

// NPC ctor
Npc::Npc(int new_npc_type,Settings& currentSettings,Maps& currentLevel) : Character(currentSettings,currentLevel)
{
	//Test
	destX=0;
	destY=0;
	
	//Init
	playerSpotted=false;

	//Get all item objects
	Items empty(0);
	Items sword(1);
	Items shield(2);

	//Create default inventories
	Items enemyBag();
	Items vendorBag();

	switch(new_npc_type)
	{
	//Vendors
	case 31:
		name="Vendor";
		character_type=new_npc_type;
		icon='v';
		attackDmg=0;
		skill=0;
		health=5;
		coins=100;
		for(int k=0;k<22;k++){for(int i=0;i<42;i++){if(currentLevel.map[k][i]==character_type)setLocation(k,i);}}
		harm=false;
		break;
	case 32:
		name="Vendor";
		character_type=new_npc_type;
		icon='V';
		attackDmg=0;
		skill=0;
		health=5;
		coins=100;
		for(int k=0;k<22;k++){for(int i=0;i<42;i++){if(currentLevel.map[k][i]==character_type)setLocation(k,i);}}
		harm=false;
		break;

	//Harm NPCS
	case 41:
		name="Easy Enemy";
		character_type=new_npc_type;
		icon=101;
		attackDmg=2;
		skill=6;
		health=6;
		coins=5;
		for(int k=0;k<22;k++){for(int i=0;i<42;i++){if(currentLevel.map[k][i]==character_type)setLocation(k,i);}}
		harm=true;
		break;

	case 42:
		name="Hard Enemy";
		character_type=new_npc_type;
		icon=69;
		attackDmg=2;
		skill=8;
		health=8;
		coins=20;
		for(int k=0;k<22;k++){for(int i=0;i<42;i++){if(currentLevel.map[k][i]==character_type)setLocation(k,i);}}
		harm=true;
		break;
	default:
		break;
	}
}

// (1/chance probability) for NPC to wander a random direction
void Npc::wander(int chance,Maps& currentLevel)
{
	//Initialize
	int dir=0;
	srand(time(0));
	int allow=(1+(rand()%chance));

	//Chance to wander
	if(allow==1)
	{
		//Find direction that isn't a collision
		while(1)
		{
			dir=(1+(rand()%8));
			if(okayToMove(dir,currentLevel.map))
			{
				break;
			}
			else continue;
		}
		//Move dir
		move(dir,&currentLevel,false);
	}
	return;
}

// NPC Goes toward C1
void Npc::goToCharacter(Character& c1,Maps& currentLevel)
{
	int bestStep=findBestStep(c1.x,c1.y);
	int*path=pathClear(c1,&currentLevel);
	int*navi=navigateToOpening(c1,&currentLevel,path);
	destX=path[0];
	destY=path[1];
	//Default return value for PathClear. This means the path is clear.
	if((path[0]==50)&&(path[1]==50))
	{
		playerSpotted=true;
		if(okayToMove(bestStep,currentLevel.map))
		{
			move(bestStep,&currentLevel,false);
		}
	}
	else if (((navi[0]!=50)&&(navi[1]!=50))&&playerSpotted)
	{
		goToCoords(navi[0],navi[1],currentLevel);
	}
	else wander(3,currentLevel);
}

// Character moves toward a given set of coordinates
void Npc::goToCoords(int coord_x,int coord_y,Maps& currentLevel)
{
	int bestStep=findBestStep(coord_x,coord_y);
	if(okayToMove(bestStep,currentLevel.map))
	{
		move(bestStep,&currentLevel,false);
	}
}

// Allows NPC to find a door opening --- fix
int* Npc::navigateToOpening(Character& c1,Maps* currentLevel,int* coords)
{
	//Set coords to wall
	int bestStep=findBestStep(c1.x,c1.y);
	int initialX=coords[0];
	int initialY=coords[1];

	int temp_x=initialX;
	int temp_y=initialY;

	int * naviCoords=new int[2];
	naviCoords[0]=50;
	naviCoords[1]=50;
	//If best step is a vertical step collision, then the wall must be a horizantal wall.
	//If best step is a horizantal step collision, then the wall must be a vertical wall.
	bool loopVertical=false;
	switch(bestStep)
	{
	case UP:
		temp_y--;
		loopVertical=false;
		break;
	case DOWN:
		temp_y++;
		loopVertical=false;
		break;
	case RIGHT:
		temp_x++;
		loopVertical=true;
		break;
	case LEFT:
		temp_x--;
		loopVertical=false;
		break;
	default:
		break;
	}
	
	//Loop through wall map, if opening is found, move to said opening.
	for(int i=0;i<4;i++)
	{
		if(loopVertical)
		{
			if(currentLevel->map[temp_y][temp_x]==1 || currentLevel->map[temp_y][temp_x]==6 || currentLevel->map[-temp_y][temp_x]==1 || currentLevel->map[-temp_y][temp_x]==6)
			{
				temp_y++;
			}
			else if(currentLevel->map[temp_y][temp_x]==0 || currentLevel->map[-temp_y][temp_x]==0)
			{
				naviCoords[0]=temp_x;
				naviCoords[1]=temp_y;
				return naviCoords;
			}
			else return naviCoords;
		}
		else
		{
			if(currentLevel->map[temp_y][temp_x]==1 || currentLevel->map[temp_y][temp_x]==6 || currentLevel->map[temp_y][-temp_x]==1 || currentLevel->map[temp_y][-temp_x]==6)
			{
				temp_x++;
			}
			else if(currentLevel->map[temp_y][temp_x]==0 || currentLevel->map[temp_y][-temp_x]==0)
			{
				naviCoords[0]=temp_x;
				naviCoords[1]=temp_y;
				return naviCoords;
			}
			else return naviCoords;
		}
	}
	return naviCoords;
}

//Finds best direction to move toward Character c1
int Npc::findBestStep(int spot_x,int spot_y) 
{
    double bestDist=findDistance(spot_x,spot_y,x,y);
    double downDist=findDistance(spot_x,spot_y,x,y+1);
    double upDist=findDistance(spot_x,spot_y,x,y-1);
    double leftDist=findDistance(spot_x,spot_y,x-1,y);
    double rightDist=findDistance(spot_x,spot_y,x+1,y);

    //Find dist alg
    if(upDist<bestDist)
    {
        if(rightDist<upDist)
        {
            return RIGHT;
        }
        else if(leftDist<upDist)
        {
            return LEFT;
        }
        else return UP;
    }

    else if(downDist<bestDist)
    {
        if(rightDist<downDist)
        {
            return RIGHT;
        }
        else if(leftDist<downDist)
        {
            return LEFT;
        }
        else return DOWN;
    }
    else if(rightDist<bestDist)
    {
        if(upDist<rightDist)
        {
            return UP;
        }
        else if(downDist<rightDist)
        {
            return DOWN;
        }
        else return RIGHT;
    }
    else if(leftDist<bestDist)
    {
        if(upDist<leftDist)
        {
            return UP;
        }
        else if(downDist<leftDist)
        {
            return DOWN;
        }
        else return LEFT;
    }
    return 0;
}

//Finds the distance between NPC and Player
double Npc::findDistance(int spot_x,int spot_y,int nX,int nY)
{
    double a=abs(spot_x-nX);
    double b=abs(spot_y-nY);
    return sqrt((a*a)+(b*b));
}

//Simulates movement to player, if path is clear, goto player, else return the value of virtual wall hit.
int* Npc::pathClear(Character& c1,Maps* currentLevel)
{
	int oldX=x;
	int oldY=y;
	int bestStep=0;
	int * pCoords=new int[2];
	pCoords[0]=50;
	pCoords[1]=50;
	while(findDistance(c1.x,c1.y,x,y)!=1)
	{
		bestStep=findBestStep(c1.x,c1.y);
		if(okayToMove(bestStep,currentLevel->map))
		{
			move(bestStep,currentLevel,true);
			if(adjacent(c1,*currentLevel))
			{
				x=oldX;
				y=oldY;
				return pCoords;
			}
		}
		else
		{
			x=oldX;
			y=oldY;
			pCoords[0]=x;
			pCoords[1]=y;
			return pCoords;
		}
	}
	return pCoords;
}

//Base character attacks target character.
void Npc::attack(Character* enemy,Maps* currentLevel)
{
	Game::Sound gameSounds;
	int randProb=0;
	srand((unsigned int)time(0));
	randProb=(1+rand()%10);
	if(randProb>skill)
	{
		gameSounds.attackSound();
		currentLevel->message=("An enemy attacked you!");
		enemy->health=enemy->health-attackDmg;
	}
	if(enemy->health<1)
	{
		enemy->alive=false;
	}
}
