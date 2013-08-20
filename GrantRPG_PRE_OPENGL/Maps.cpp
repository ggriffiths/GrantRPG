	//Maps.cpp
#include "Maps.h"

//--Project Headers--//
#include "Game.h"
#include "GameMenu.h"
#include "Items.h"
#include "Character.h"
#include "Settings.h"
#include "Player.h"
#include "Npc.h"

//--Language Headers--//
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include <ctime>

//--Namespaces--//
using namespace std;
using namespace System;

//--Global Variables--//
enum keys{UP=5,DOWN,RIGHT,LEFT};

// ctor for new map
Maps::Maps(int new_level,int new_world,Settings& currentSettings)
: 
cmc(new_world,new_level),
n1(31,currentSettings,*this),
e1(41,currentSettings,*this),
e2(42,currentSettings,*this),
empty(0),sword(1),shield(2)
{
	level=new_level;
	world=new_world;
	message="Use WASD or the Arrow Keys to move.";
	old_message=message;
	message2="";
	message3="";

	//Update NPCs after map creation
	Npc new_n1(31,currentSettings,*this);
	n1=new_n1;

	Npc new_e1(41,currentSettings,*this);
	e1=new_e1;

	Npc new_e2(42,currentSettings,*this);
	e2=new_e2;
}

// ctor for loaded map
Maps::Maps(char new_map[22][42],int new_level,int new_world,Settings& currentSettings,Character* p1) 
: 
cmc(new_world,new_level),
n1(31,currentSettings,*this),
e1(41,currentSettings,*this),
e2(42,currentSettings,*this),
empty(0),sword(1),shield(2)
{
	level=new_level;
	world=new_world;
	message="Use WASD or the Arrow Keys to move.";
	old_message=message;
	message2="";
	message3="";
	for(int k=0;k<22;k++)
	{
		for(int i=0;i<42;i++)
		{
			map[k][i]=new_map[k][i];
		}
	}

	//Update NPCs after map creation
	Npc new_n1(31,currentSettings,*this);
	n1=new_n1;

	Npc new_e1(41,currentSettings,*this);
	e1=new_e1;

	Npc new_e2(42,currentSettings,*this);
	e2=new_e2;

	
}

// Copies a Maps object
Maps Maps::operator=(Maps& sourceMap)
{
	return sourceMap;
}

// Gets privately located map values
char Maps::getMapValue(int k, int i)
{
	int num=storedMapValues(world,level,k,i);
	return num;
}

// Stores private map values
char Maps::storedMapValues(int world,int level,int k, int i)
{
	const char a=10;
	const char b=11;
	const char m=31;
	const char n=32;
	const char u=41;
	const char v=42;
	int num=0;
	if(world==1) // World 1
	{
		if(level==1) // Level 1:1
		{
			char levelOne[22][42]=
			{
				1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
				1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,
				1,1,1,1,1,0,3,0,0,3,3,0,0,3,0,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,
				1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,2,0,0,6,6,0,0,0,0,0,0,0,0,0,6,6,0,0,9,0,1,1,
				1,1,1,1,1,1,1,6,6,6,6,6,6,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,a,a,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,a,a,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,a,a,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,m,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,a,a,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,a,a,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,a,a,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,b,b,b,b,b,b,6,6,6,b,b,b,b,b,b,a,a,6,6,6,6,6,a,a,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,v,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,u,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,4,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,7,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,0,0,b,b,0,0,0,0,0,0,0,0,0,1,1,
				1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,1,1,1,1,1,1,1,1,1,1,1
			};
			num=levelOne[k][i];
		}
		else if(level==2) // Level 1:2
		{
			char levelTwo[22][42]=
			{
				1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,v,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,u,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
				1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
			};
			num=levelTwo[k][i];
		}	
	}
	return num;
}

// Prints game to the console
void Maps::printMap(Player& p1)
{
	//Map Icons
	char walli=178;
	char scorei=155;
	char upi=24;
	char downi=25;
	char spacei=32;
	char door=186;
	char sword=159;
	char heart=3;
	char shield=233;
	char fence=176;
	char solid_wall=178;

	//Initialize Map
		std::system("CLS");
	cmc.setColor(0x0e);

	//Print Notification
	if(message!=old_message)
	{
		message3=message2;
		message2=old_message;
		old_message=message;
	}
	cout << message3;
	for(int i=0;i<42-message.length();i++) cout << " ";
	cout << "\n";
	cout << message2;
	for(int i=0;i<42-message.length();i++) cout << " ";
	cout << "\n";
	cout << message;
	for(int i=0;i<42-message.length();i++) cout << " ";
	cout << "\n";

	for(unsigned int k=0;k<22;k++)
	{
		for(unsigned int i=0;i<42;i++)
		{	
			if     (map[k][i]==1) // Wall
			{
				cmc.setColor(cmc.wall);
				cout << walli;
			}	
			else if(map[k][i]==2)// Character p1->icon
			{
				cmc.setColor(cmc.player);
				cout << p1.icon;
			}
			else if(map[k][i]==3)// Coin
			{
				cmc.setColor(cmc.score);
				cout << scorei;
			}
			else if(map[k][i]==4)// Up
			{
				cmc.setColor(cmc.up);
				cout << upi;
			}
			else if(map[k][i]==5)// Down
			{
				cmc.setColor(cmc.down);
				cout << downi;
			}
			else if(map[k][i]==6)// Door 
			{
				cmc.setColor(cmc.door);
				cout << door;
			}
			else if(map[k][i]==7)// Sword
			{
				cmc.setColor(cmc.sword);
				cout << sword;
			}
			else if(map[k][i]==8)// Heart
			{
				cmc.setColor(cmc.npc_harm);
				cout << heart;
			}
			else if(map[k][i]==9)// Shield
			{
				cmc.setColor(cmc.sword);
				cout << shield;
			}
			else if(map[k][i]==10)// fence
			{
				cmc.setColor(cmc.fence);
				cout << fence;
			}
			else if(map[k][i]==11)// solid wall
			{
				cmc.setColor(cmc.solid_wall);
				cout << solid_wall;
			}
			else if(map[k][i]==n1.character_type)// Vendor N1
			{
				if(n1.alive)
				{
					cmc.setColor(cmc.npc_noharm);
					cout << n1.icon;
				}
				else // Space 32
				{
					cmc.setColor(cmc.space);
					cout << spacei;
				}
			}
			else if(map[k][i]==e2.character_type)// Enemy E2
			{
				if(e2.alive)
				{
					cmc.setColor(cmc.npc_harm);
					cout << e2.icon;
				}
				else // Space 32
				{
					cmc.setColor(cmc.space);
					cout << spacei;
				}
			}
			else if(map[k][i]==e1.character_type)// Enemy E1
			{
				if(e1.alive)
				{
					cmc.setColor(cmc.npc_harm);
					cout << e1.icon;
				}
				else // Space 32
				{
					cmc.setColor(cmc.space);
					cout << spacei;
				}
			}
			else // Space 32
			{
				cmc.setColor(cmc.space);
				cout << spacei;
			}
		}
		cout << endl;
	}
	cmc.setColor(0x1f);
	
	//Print Stats
	cout<< "\n";
	printStatistics(p1,cmc);
	cout<< "\n";
	cout << "N1 Position: (" << n1.x << "," << n1.y << ") N1_TYPE:" << n1.character_type << " N1 Health: " << n1.health << "DestX: " << n1.destX << " destY: " << n1.destY << "\n";
	cout << "E1 Position: (" << e1.x << "," << e1.y << ") E1_TYPE:" << e1.character_type << " E1 Health: " << e1.health << "DestX: " << e1.destX << " destY: " << e1.destY << "\n";
	cout << "E2 Position: (" << e2.x << "," << e2.y << ") E2_TYPE:" << e2.character_type << " E2 Health: " << e2.health << "DestX: " << e2.destX << " destY: " << e2.destY << "\n";
}

//detect if enemy is 
bool Maps::enemyWithinRange(Player& p1,Npc& enemy)
{
	if(map[p1.y-1][p1.x]==enemy.character_type) //up
	{
		return true;
	}
	else if(map[p1.y+1][p1.x]==enemy.character_type) //down
	{
		return true;
	}
	else if(map[p1.y][p1.x+1]==enemy.character_type) //right
	{
		return true;
	}
	else if(map[p1.y][p1.x-1]==enemy.character_type) //left
	{
		return true;
	}
	else return false;
}

// Prints player/level stats to the screen
void Maps::printStatistics(Player& p1,MapColors& cmc)
{
	//Output stats to bottom of screen
	//Top Bar
	cmc.setColor(0x1f);
	pSpaces(15);
	cout << "Statistics:";
	pSpaces(16);

	cout << "\n  ";
	cmc.setColor(0xf0);
		//Level
		cout << "  Level: " << world << ":" << level;
		pSpaces(26);
		cmc.setColor(0x10);
		cout << "  \n  ";
		cmc.setColor(0xf0);

		//Score
		cout << "  Coins: " << p1.coins;
		pSpaces(28);
		cmc.setColor(0x10);
		cout << "  \n  ";
		cmc.setColor(0xf0);

		//Lives
		cout << " "  << " Lives left: " << "1";
		pSpaces(23);
		cmc.setColor(0x10);
		cout << "  \n  ";
		cmc.setColor(0xf0);

		//Health
		char heart=3;
		cout << "  Health: ";
		for(int i=0;i<p1.health;i++) cout << heart << " ";
			//Handles space issue w/ number of hearts
			for (int i=0;i<(14-p1.health);i++) cout << "  ";
		cmc.setColor(0x10);
		cout << "  \n  ";
		cmc.setColor(0xf0);

		//Position
		cout << "  Position: " << p1.x << "," << p1.y;
			//Handles digit issues w/ spacing...
			if(p1.x>9&&p1.y>9)        pSpaces(21);
			else if(p1.x>9||p1.y>9)   pSpaces(22);
			else if(p1.x<10&&p1.y<10) pSpaces(23);
		cmc.setColor(0x10);
		cout << "  ";

	//Bottom Bar
	cout << "\n";
	pSpaces(42);
	cout << "\n";

	// Back to normal color
	cmc.setColor(0x0f);
}

//Settings for next level... 
void Maps::advanceLevel()
{
	level++;
	for(int k=0;k<22;k++)
	{
		for(int i=0;i<42;i++)
		{
			map[k][i]=getMapValue(k,i);
		}
	}
}

//Settings for next world... 
void Maps::advanceWorld()
{
	level=1;
	world++;
}

//Simple spacer method
void Maps::pSpaces(int size)
{
	for(int i=0;i<size;i++) cout << " ";
}

// Colors
//Format:
//0 x 2       f
//0 x bgColor fgColor 
//0 x GreenBG WhiteFG

//0 = Black	  8 = Gray
//1 = Blue	  9 = Light Blue
//2 = Green   a = Light Green
//3 = Aqua	  b = Light Aqua
//4 = Red	  c = Light Red
//5 = Purple  d = Light Purple
//6 = Yellow  e = Light Yellow
//7 = White	  f = Bright White

//Sets colors based on World,level ints (1:1),(1:2),...(w,l)
Maps::MapColors::MapColors(int world,int level)
{
	//Level Themes
	switch(world)
	{
		case 1:
			switch(level) // World 1: Grassy/Bright
			{
			case 1: //Level 1:1
				wall=0x2f;
				score=0x2e;
				player=0x21;
				up=0x2e;
				down=0x2e;
				space=0x20;
				door=0x20;
				sword=0x21;
				npc_noharm=0x2f;
				npc_harm=0x2c;
				fence=0x20;
				solid_wall=0x20;
				break;
			case 2: //Level 1:2
				wall=0x2f;
				score=0x2e;
				player=0x21;
				up=0x20;
				down=0x20;
				space=0x20;
				door=0x20;
				sword=0x21;
				npc_noharm=0x2f;
				npc_harm=0x2c;
				fence=0x20;
				solid_wall=0x20;
				break;
			case 3: //Level 1:3
				wall=0x2f;
				score=0x2e;
				player=0x21;
				up=0x20;
				down=0x20;
				space=0x20;
				door=0x20;
				sword=0x21;
				npc_noharm=0x2f;
				npc_harm=0x2c;
				fence=0x20;
				solid_wall=0x20;
				break;
			};
			break;
		case 2:
			switch(level) // World 2: Cave Dark(until flashlight found)
			{
			case 1: // Level 2:1
				wall=0x2f;
				score=0x2e;
				player=0x21;
				up=0x20;
				down=0x20;
				space=0x20;
				door=0x20;
				sword=0x21;
				npc_noharm=0x2f;
				npc_harm=0x2c;
				fence=0x26;
				solid_wall=0x20;
				break;
			case 2: // Level 2:2
				wall=0x2f;
				score=0x2e;
				player=0x21;
				up=0x20;
				down=0x20;
				space=0x20;
				door=0x20;
				sword=0x21;
				npc_noharm=0x2f;
				npc_harm=0x2c;
				fence=0x26;
				solid_wall=0x2f;
				break;
			case 3: // Level 2:3
				wall=0x2f;
				score=0x2e;
				player=0x21;
				up=0x20;
				down=0x20;
				space=0x20;
				door=0x20;
				sword=0x21;
				npc_noharm=0x2f;
				npc_harm=0x2c;
				fence=0x26;
				solid_wall=0x28;
				break;
			};
			break;
	}
}

//Set color of text
void Maps::MapColors::setColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Handles possible NPC movement
void Maps::handleNpcMovement(Player& p1,Maps* currentLevel)
{
	if(n1.alive) n1.wander(5,*currentLevel);
	if(e1.alive) e1.goToCharacter(p1,*currentLevel);	
	if(e2.alive) e2.goToCharacter(p1,*currentLevel);
}

//Handles possible NPC actions
void Maps::handleNpcAction(Character* p1,Maps* currentLevel)
{
	//If Player is near NPC, attack player.
	if(e1.adjacent(*p1,*this)) e1.attack(p1,currentLevel);
	if(e2.adjacent(*p1,*this)) e2.attack(p1,currentLevel);
}

