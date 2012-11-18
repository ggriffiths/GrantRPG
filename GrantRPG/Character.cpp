//--Character.cpp--//
#include "Character.h"

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

//Character constructor (zero params)
Character::Character() : inv(false)
{

}

//Character constructor! (w/ params)
Character::Character(int new_health,int new_x,int new_y,int new_coins,char new_icon,WORD new_color,bool new_harm,Settings& currentSettings) : inv(false)
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

//Character constructor! (defaults)
Character::Character(Settings& currentSettings,Maps& currentLevel) : inv(false)
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

//Updates Character position based on direction & Character1 obj
void Character::move(int dir,Maps* currentLevel,bool temp)
{
	Game::Sound gameSounds;
	if(name=="Player") gameSounds.moveSound();
	if(okayToMove(dir,currentLevel->map))
	{
		if(!temp) currentLevel->map[y][x]=0;
		switch(dir)
		{
			case 1: // upright
				y--;
				x++;
				break;
			case 2: // upleft
				y--;
				x--;
				break;
			case 3: // downright
				x++;
				y++;
				break;
			case 4: // downleft
				x--;
				y++;
				break;
			case 5: // up
				y--;
				break;
			case 6: // down
				y++;
				break;
			case 7: // right
				x++;
				break;
			case 8: // left
				x--;
				break;
		}
		if(name=="Player")
		{
			if(currentLevel->map[y][x]==3)
			{
				gameSounds.getCoin();
				currentLevel->message="You've picked up a Coin!";
				addCoin();
			}
			if(currentLevel->map[y][x]==4)
			{
				currentLevel->advanceLevel();
				gameSounds.advanceLevel();
				Settings nextSettings(true,0,0,false,false);
				Maps nextLevel(currentLevel->level,currentLevel->world,nextSettings);
				*currentLevel=nextLevel;
			}
			else if(currentLevel->map[y][x]==7)
			{
				gameSounds.getItem();
				currentLevel->message="You've picked up a Sword!";
				inv.addItem(currentLevel->sword);
			}
			else if(currentLevel->map[y][x]==8)
			{
				gameSounds.getItem();
				currentLevel->message="You've picked up a heart!";
				health++;
			}
			else if(currentLevel->map[y][x]==9)
			{
				gameSounds.getItem();
				currentLevel->message="You've picked up a Shield!";
				inv.addItem(currentLevel->shield);
			}
		}
		if(!temp) currentLevel->map[y][x]=character_type;
		//Sleep(gameSpeed);
	}
	else
	{
		gameSounds.collision();
	}
}

//Performs action
void Character::doAction(Maps* currentLevel)
{
	Sleep(100);
	if(currentLevel->map[y-1][x]==6) //up
	{ 
		currentLevel->message="Door unlocked!";
		unlockDoor(UP,currentLevel);
	}
	else if(currentLevel->map[y+1][x]==6) //down
	{
		currentLevel->message="Door unlocked!";
		unlockDoor(DOWN,currentLevel);
	}
	else if(currentLevel->map[y][x+1]==6) //right
	{
		currentLevel->message="Door unlocked!";
		unlockDoor(RIGHT,currentLevel);
	}
	else if(currentLevel->map[y][x-1]==6) //left
	{
		currentLevel->message="Door unlocked!";
		unlockDoor(LEFT,currentLevel);
	}

	//Talk to vendor detection
	else if(currentLevel->map[y-1][x]==31) //up
	{
		currentLevel->message="You just talked to a vendor!";
		vendorScreen(currentLevel->n1.inv);
	}
	else if(currentLevel->map[y+1][x]==31) //down
	{
		currentLevel->message="You just talked to a vendor!";
		vendorScreen(currentLevel->n1.inv);
	}
	else if(currentLevel->map[y][x+1]==31) //right
	{
		currentLevel->message="You just talked to a vendor!";
		vendorScreen(currentLevel->n1.inv);
	}
	else if(currentLevel->map[y][x-1]==31) //left
	{
		currentLevel->message="You just talked to a vendor!";
		vendorScreen(currentLevel->n1.inv);
	}	
}

// Virtual Function for polymorphic character attacking.
void Character::attack(Character* enemy,Maps* currentLevel)
{

}

//Checks if base Character is adjacent to given Character
bool Character::adjacent(Character& p1,Maps& currentLevel)
{
	if(currentLevel.map[y+1][x]==p1.getType())
	{
		return true;
	}
	else if(currentLevel.map[y][x+1]==p1.getType())
	{
		return true;
	}
	else if(currentLevel.map[y-1][x]==p1.getType())
	{
		return true;
	}
	else if(currentLevel.map[y][x-1]==p1.getType())
	{
		return true;
	}
	else return false;
}

//Returns the player type
char Character::getType()
{
	return character_type;
}

//Unlocks multiple adjacent doors based on player position
void Character::unlockDoor(int dirUnlock,Maps* currentLevel)
{
	bool t1=false;
	bool t2=false;
	Game::Sound gameSounds;
	gameSounds.activate();
	switch(dirUnlock)
	{
		case 5: // up
			currentLevel->map[y-1][x]=0;
			for(int k=1;k<8;k++)
			{
				for(int i=0;i<8;i++) 
				{
					if(currentLevel->map[y-k][x+i]==6)
					{
						currentLevel->map[y-k][x+i]=0;
					}
					else t1=true;
					if(currentLevel->map[y-k][x-i]==6)
					{
						currentLevel->map[y-k][x-i]=0;
					}
					else t2=true;
				}
				if(t1&&t2) break;
			}
			break;
		case 6: // down
			currentLevel->map[y+1][x]=0;
			for(int k=1;k<8;k++)
			{
				for(int i=0;i<8;i++) 
				{
					if(currentLevel->map[y+k][x+i]==6)
					{
						currentLevel->map[y+k][x+i]=0;
					}
					else t1=true;
					if(currentLevel->map[y+k][x-i]==6)
					{
						currentLevel->map[y+k][x-i]=0;
						
					}
					else t2=true;
				}
				if(t1&&t2) break;
			}
			break;
		case 7: // right
			currentLevel->map[y][x+1]=0;
			for(int k=1;k<7;k++)
			{
				for(int i=0;i<6;i++) 
				{
					if(currentLevel->map[y+i][x+k]==6)
					{
						currentLevel->map[y+i][x+k]=0;
					}
					else t1=true;
					if(currentLevel->map[y-i][x+k]==6)
					{
						currentLevel->map[y-i][x+k]=0;
					}
					else t2=true;
					if(t1&&t2) break;
				}
			}
			break;
		case 8: // left
			currentLevel->map[y][x-1]=0;
			for(int k=1;k<7;k++)
			{
				for(int i=0;i<6;i++) 
				{
					if(currentLevel->map[y+i][x-k]==6)
					{
						currentLevel->map[y+i][x-k]=0;
					}
					else t1=true;
					if(currentLevel->map[y-i][x-k]==6)
					{
						currentLevel->map[y-i][x-k]=0;
					}
					else t2=true;
					if(t1&&t2) break;
				}
			}
			break;
		default:
			break;
	}
}

//Gives the Character a coin.
void Character::addCoin()
{
	coins++;
}


//Holds objects of items!
Character::Inventory::Inventory(bool loadFromFile) : bag()
{
	if(loadFromFile)
	{
		// load inv alg
	}
	else
	{
		numItemsHolding=0;
	}
}

//Prints inventory to screen.
void Character::Inventory::printInventory(bool player)
{
	
	//Top Bar
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1f);
	
	
	if(player)
	{
		pSpaces(12);
		cout << "Player Inventory:";
		pSpaces(13);
	}
	else 
	{
		pSpaces(14);
		cout << "NPC Inventory:";
		pSpaces(14);
	}
	cout << "\n  ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
	pSpaces(38);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1f);
	cout << "  \n  ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);

	//Line 1 (6 Items)
	for(int i=0;i<12;i++)
	{
		// line space after 6
		if((i%6)==0)
		{			
			//Spacer...
			pSpaces(2);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1f);
			cout << "  \n  ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
			pSpaces(38);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1f);
			cout << "  \n  ";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
		}
		cout << "  " << bag[i].icon;
		if(bag[i].quantity>1) cout << "(" <<bag[i].quantity << ")";
		else cout << "   ";
		
	}
			
	//Bottom Bar
	pSpaces(2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1f);
	cout << "  \n  ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
	pSpaces(38);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1f);
	cout << "  \n";
	pSpaces(42);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
	cout << "\n";

	unsigned char coin=155;
	if(player) cout << "Your items: ";
	else  cout << "NPC items: ";
	for(int i=0;i<12;i++)
	{
		if(bag[i].name!="") cout << bag[i].name << "(" << coin << bag[i].value << ")" << ", ";
	}
	cout << "\n";
}

//Simple spacer method
void Character::Inventory::pSpaces(int size)
{
	for(int i=0;i<size;i++) cout << " ";
}

//Simply sets an item to an inventory slot.
void Character::Inventory::setItem(Items& new_item,int spot)
{
	bag[spot] = new_item;
}

//Adds item to inventory. If item already exists in inventory, it adds to the stack.
void Character::Inventory::addItem(const Items& new_item)
{
	int invSlot=getSpotOfItemInInv(new_item);
	if(invSlot>0)
	{
		bag[invSlot].quantity++;
	}
	else if(invSlot==0)
	{
		numItemsHolding++;
		bag[numItemsHolding]=new_item;
	}
}

//Finds the location of an item in the inventory.
int Character::Inventory::getSpotOfItemInInv(const Items& new_item)
{
	for(int i=0;i<12;i++)
	{
		if(bag[i].icon==new_item.icon)
		{
			return (i+1);
		}
	}
	return 0;
}

//Walks user through vendor screen with npc
void Character::vendorScreen(Inventory& vendor_inv)
{
	system("CLS");
	Beep(250,250);
	Beep(300,250);
	Beep(400,500);
	Sleep(250);
	string input="";
	system("CLS");
	bool buy=false;
	bool quit=false;
	bool invalid=false;
	string notEnough="false";
	//Find if sell or buy
	while(1)
	{
		while(1)
		{
			system("CLS");
			vendor_inv.printInventory(false);
			cout << "\n";
			inv.printInventory(true);
			quit=false;
			if(invalid) cout << "\nI'm sorry, I didn't understand that answer.\nPossible answers: Buy,buy,B,b,Sell,sell,S,s,X,x\n";
			else cout << "\nGreetings traveler, what would you like to do? (Buy/Sell) or (X) to exit.";
			cin >> input;
			if(input=="Buy"||input=="buy"||input=="b"||input=="B")
			{
				invalid=false;
				buy=true;
				break;
			}
			else if(input=="Sell"||input=="sell"||input=="s"||input=="S")
			{
				invalid=false;
				buy=false;
				break;
			}
			else if(input=="X"||input=="x")
			{
				quit=true;
				break;
			}
			else invalid=true;
		}
		while(1)
		{
			if(quit) break;
			else if(buy)
			{
				system("CLS");
				vendor_inv.printInventory(false);
				cout << "\n";
				inv.printInventory(true);
				if(invalid) cout << "\nI'm sorry, I didn't understand that answer.\nEnter an item that the NPC has in it's inventory.\n";
				else cout << "\nWhat would you like to Buy? Enter an item name OR (X) to exit:\n";
				if(notEnough!="false"&&!invalid) cout << "You don't have enough coins to buy a(n) " << notEnough << ". Enter another item name:\n";
				cin >> input;
				if(input=="")
				{
					invalid=true;
				}
				else if(input==vendor_inv.bag[0].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[0].value)
					{
						cout << "\n" << vendor_inv.bag[0].name << "bought for " << vendor_inv.bag[0].value << " coins!\n";
						coins=coins-vendor_inv.bag[0].value;
						inv.addItem(vendor_inv.bag[0]);
						break;
					}
					else notEnough=vendor_inv.bag[0].name;
				}
				else if(input==vendor_inv.bag[1].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[1].value)
					{
						cout << "\n" << vendor_inv.bag[1].name << "bought for " << vendor_inv.bag[1].value << " coins!\n";
						coins=coins-vendor_inv.bag[1].value;
						inv.addItem(vendor_inv.bag[1]);
						break;
					}
					else notEnough=vendor_inv.bag[1].name;
				}
				else if(input==vendor_inv.bag[2].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[2].value)
					{
						cout << "\n" << vendor_inv.bag[2].name << "bought for " << vendor_inv.bag[2].value << " coins!\n";
						coins=coins-vendor_inv.bag[2].value;
						inv.addItem(vendor_inv.bag[2]);
						break;
					}
					else notEnough=vendor_inv.bag[2].name;
				}
				else if(input==vendor_inv.bag[3].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[3].value)
					{
						cout << "\n" << vendor_inv.bag[3].name << "bought for " << vendor_inv.bag[3].value << " coins!\n";
						coins=coins-vendor_inv.bag[3].value;
						inv.addItem(vendor_inv.bag[3]);
						break;
					}
					else notEnough=vendor_inv.bag[3].name;
				}
				else if(input==vendor_inv.bag[4].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[4].value)
					{
						cout << "\n" << vendor_inv.bag[4].name << "bought for " << vendor_inv.bag[4].value << " coins!\n";
						coins=coins-vendor_inv.bag[4].value;
						inv.addItem(vendor_inv.bag[4]);
						break;
					}
					else notEnough=vendor_inv.bag[4].name;
				}
				else if(input==vendor_inv.bag[5].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[5].value)
					{
						cout << "\n" << vendor_inv.bag[5].name << "bought for " << vendor_inv.bag[5].value << " coins!\n";
						coins=coins-vendor_inv.bag[5].value;
						inv.addItem(vendor_inv.bag[5]);
						break;
					}
					else notEnough=vendor_inv.bag[5].name;
				}
				else if(input==vendor_inv.bag[6].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[6].value)
					{
						cout << "\n" << vendor_inv.bag[6].name << "bought for " << vendor_inv.bag[6].value << " coins!\n";
						coins=coins-vendor_inv.bag[6].value;
						inv.addItem(vendor_inv.bag[6]);
						break;
					}
					else notEnough=vendor_inv.bag[6].name;
				}
				else if(input==vendor_inv.bag[7].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[7].value)
					{
						cout << "\n" << vendor_inv.bag[7].name << "bought for " << vendor_inv.bag[7].value << " coins!\n";
						coins=coins-vendor_inv.bag[7].value;
						inv.addItem(vendor_inv.bag[7]);
						break;
					}
					else notEnough=vendor_inv.bag[7].name;
				}
				else if(input==vendor_inv.bag[8].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[8].value)
					{
						cout << "\n" << vendor_inv.bag[8].name << "bought for " << vendor_inv.bag[8].value << " coins!\n";
						coins=coins-vendor_inv.bag[8].value;
						inv.addItem(vendor_inv.bag[8]);
						break;
					}
					else notEnough=vendor_inv.bag[8].name;
				}
				else if(input==vendor_inv.bag[9].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[9].value)
					{
						cout << "\n" << vendor_inv.bag[9].name << "bought for " << vendor_inv.bag[9].value << " coins!\n";
						coins=coins-vendor_inv.bag[9].value;
						inv.addItem(vendor_inv.bag[9]);
						break;
					}
					else notEnough=vendor_inv.bag[9].name;
				}
				else if(input==vendor_inv.bag[10].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[10].value)
					{
						cout << "\n" << vendor_inv.bag[10].name << "bought for " << vendor_inv.bag[10].value << " coins!\n";
						coins=coins-vendor_inv.bag[10].value;
						inv.addItem(vendor_inv.bag[10]);
						break;
					}
					else notEnough=vendor_inv.bag[10].name;
				}
				else if(input==vendor_inv.bag[11].name)
				{
					notEnough="false";
					if(coins>vendor_inv.bag[11].value)
					{
						cout << "\n" << vendor_inv.bag[11].name << "bought for " << vendor_inv.bag[11].value << " coins!\n";
						coins=coins-vendor_inv.bag[11].value;
						inv.addItem(vendor_inv.bag[11]);
						break;
					}
					else notEnough=vendor_inv.bag[11].name;
				}
				else if(input=="x" || input=="X") break;
				else 
				{
					invalid=true;
					notEnough="false";
				}
			}
			else if(!buy)
			{
				system("CLS");
				vendor_inv.printInventory(false);
				cout << "\n";
				inv.printInventory(true);
				if(invalid) cout << "\nI'm sorry, I didn't understand that answer.\nEnter an item that you have in your inventory.\n";
				else cout << "\nWhat would you like to Sell? Enter an item name OR (X) to exit:\n";
				cin >> input;
				if(input=="")
				{
					invalid=true;
				}
				else if(input==inv.bag[1].name)
				{
					cout << "\n" << inv.bag[1].name << " sold for " << inv.bag[1].value << " coins!\n";
					coins=coins+inv.bag[1].value;
					inv.removeItem(1);
					break;
				}
				else if(input==inv.bag[1].name)
				{
					cout << "\n" << inv.bag[1].name << " sold for " << inv.bag[1].value << " coins!\n";
					coins=coins+inv.bag[1].value;
					inv.removeItem(2);
					break;
				}
				else if(input==inv.bag[2].name)
				{
					cout << "\n" << inv.bag[2].name << " sold for " << inv.bag[2].value << " coins!\n";
					coins=coins+inv.bag[2].value;
					inv.removeItem(3);
					break;
				}
				else if(input==inv.bag[3].name)
				{
					cout << "\n" << inv.bag[3].name << " sold for " << inv.bag[3].value << " coins!\n";
					coins=coins+inv.bag[3].value;
					inv.removeItem(4);
					break;
				}
				else if(input==inv.bag[4].name)
				{
					cout << "\n" << inv.bag[4].name << " sold for " << inv.bag[4].value << " coins!\n";
					coins=coins+inv.bag[4].value;
					inv.removeItem(5);
					break;
				}
				else if(input==inv.bag[5].name)
				{
					cout << "\n" << inv.bag[5].name << " sold for " << inv.bag[5].value << " coins!\n";
					coins=coins+inv.bag[5].value;
					inv.removeItem(6);
					break;
				}
				else if(input==inv.bag[6].name)
				{
					cout << "\n" << inv.bag[6].name << " sold for " << inv.bag[6].value << " coins!\n";
					coins=coins+inv.bag[6].value;
					inv.removeItem(7);
					break;
				}
				else if(input==inv.bag[7].name)
				{
					cout << "\n" << inv.bag[7].name << " sold for " << inv.bag[7].value << " coins!\n";
					coins=coins+inv.bag[7].value;
					inv.removeItem(8);
					break;
				}
				else if(input==inv.bag[8].name)
				{
					cout << "\n" << inv.bag[8].name << " sold for " << inv.bag[8].value << " coins!\n";
					coins=coins+inv.bag[8].value;
					inv.removeItem(9);
					break;
				}
				else if(input==inv.bag[9].name)
				{
					cout << "\n" << inv.bag[9].name << " sold for " << inv.bag[9].value << " coins!\n";
					coins=coins+inv.bag[9].value;
					inv.removeItem(10);
					break;
				}
				else if(input==inv.bag[10].name)
				{
					cout << "\n" << inv.bag[10].name << " sold for " << inv.bag[10].value << " coins!\n";
					coins=coins+inv.bag[10].value;
					inv.removeItem(11);
					break;
				}
				else if(input==inv.bag[11].name)
				{
					cout << "\n" << inv.bag[11].name << " sold for " << inv.bag[11].value << " coins!\n";
					coins=coins+inv.bag[11].value;
					inv.removeItem(12);
					break;
				}
				else if(input=="x" || input=="X") break;
				else 
				{
					invalid=true;
				}
			}
		}
		while(1)
		{
			cout << "\nIs that all?(y/n)";
			cin >> input;
			if(input=="Y" || input== "y")
			{
				return;
			}
			if(input=="N" || input== "n")
			{
				break;
			}
		}
		continue;
	}
}

//Lowers item quantity by 1 and removes the item if quanitity==0
void Character::Inventory::removeItem(int spot)
{
	Items empty(0);
	bag[spot].quantity--;
	for(int i=0;i<12;i++)
	{
		if(bag[i].quantity==0) bag[i]=empty;
	}
}

//Checks for collision
bool Character::okayToMove(int dir,char map[22][42]) //1,2,6,31,32,41,42
{
	if(checkMapValue(1,dir,map)&&
	checkMapValue(2,dir,map)&&
	checkMapValue(6,dir,map)&&
	checkMapValue(10,dir,map)&&
	checkMapValue(11,dir,map)&&
	checkMapValue(31,dir,map)&&
	checkMapValue(32,dir,map)&&
	checkMapValue(41,dir,map)&&
	checkMapValue(42,dir,map)) 
	{
		if(name!="Player" && !checkMapValue(4,dir,map))
		{
			return false;
		}
		return true;
	}
	else return false;
}

//Checks for specific collisions
bool Character::checkMapValue(int value,int dir,char map[22][42])
{
	switch(dir)
	{
	case 1: // upright
		if(map[y-1][x+1]!=value) return true;
		else return false;
		break;
	case 2: // upleft
		if(map[y-1][x-1]!=value) return true;
		else return false;
		break;
	case 3: // downright
		if(map[y+1][x+1]!=value) return true;
		else return false;
		break;
	case 4: // downleft
		if(map[y+1][x-1]!=value) return true;
		else return false;
		break;
	case 5: // up
		if(map[y-1][x]!=value) return true;
		else return false;
		break;
	case 6: // down
		if(map[y+1][x]!=value) return true;
		else return false;
		break;
	case 7: // right
		if(map[y][x+1]!=value) return true;
		else return false;
		break;
	case 8: // left
		if(map[y][x-1]!=value) return true;
		else return false;
		break;
	default: return false;
	};
	return false;
}

//Handles Player Movement 
void Character::handlePlayerMovement(int key,Maps* currentLevel)
{
	Items shield(2);
	// Move
	for(int i=1;i<=8;i++)
	{
		if(key==i)
		{
			move(key,currentLevel,false);
			break;
		}
	}
}

//Handles Player Actions
void Character::handlePlayerAction(int key,Maps* currentLevel)
{
	Settings testSettings(true,0,0,false,false);
	Character* characterToAttack=&currentLevel->e1;
	int attackDamage=0;
	// Action key - E
	if(key==E) 
	{
		doAction(currentLevel);
	}
	else if(key==C)
	{
		if(adjacent(currentLevel->e1,*currentLevel))
		{
			characterToAttack=&(currentLevel->e1);
			attack(characterToAttack,currentLevel);
		}
		if(adjacent(currentLevel->e2,*currentLevel))
		{
			characterToAttack=&(currentLevel->e2);
			attack(characterToAttack,currentLevel);
		}
	}
	if(!alive)
	{
		system("CLS");
		cout << "You've died.\n";
		system("PAUSE");
	}
}

//Sets the X and Y values of the character.
void Character::setLocation(int k,int i)
{
	x=i;
	y=k;
}

//Shows the inventory in the console.
void Character::showInv()
{
	inv.printInventory(true);
}