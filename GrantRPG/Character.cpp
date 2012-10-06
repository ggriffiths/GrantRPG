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
Character::Inventory::Inventory(bool loadFromFile) : item1(0),item2(0),item3(0),item4(0),item5(0),item6(0),item7(0),item8(0),item9(0),item10(0),item11(0),item12(0)
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

	//Line 1 (5 Items)
		//Item 1
			cout << "  " << item1.icon;
			if(item1.quantity>1) cout << "(" <<item1.quantity << ")";
			else cout << "   ";
		//Item 2
			cout << "  " << item2.icon;
			if(item2.quantity>1) cout << "(" <<item2.quantity << ")";
			else cout << "   ";
		//Item 3
			cout << "  " << item3.icon;
			if(item3.quantity>1) cout << "(" <<item3.quantity << ")";
			else cout << "   ";
		//Item 4
			cout << "  " << item4.icon;
			if(item4.quantity>1) cout << "(" <<item4.quantity << ")";
			else cout << "   ";
		//Item 5
			cout << "  " << item5.icon;
			if(item5.quantity>1) cout << "(" <<item5.quantity << ")";
			else cout << "   ";

		//Item 6 (1 for testing..)
			cout << "  " << item6.icon;
			if(item6.quantity>1) cout << "(" <<item6.quantity << ")";
			else cout << "   ";
			

	//Spacer...
	pSpaces(2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1f);
	cout << "  \n  ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
	pSpaces(38);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1f);
	cout << "  \n  ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);


	//Line 2 (5 Items)
		//Item 1
			cout << "  " << item7.icon;
			if(item7.quantity>1) cout << "(" <<item7.quantity << ")";
			else cout << "   ";
		//Item 2
			cout << "  " << item8.icon;
			if(item8.quantity>1) cout << "(" <<item8.quantity << ")";
			else cout << "   ";
		//Item 3
			cout << "  " << item9.icon;
			if(item9.quantity>1) cout << "(" <<item9.quantity << ")";
			else cout << "   ";
		//Item 4
			cout << "  " << item10.icon;
			if(item10.quantity>1) cout << "(" <<item10.quantity << ")";
			else cout << "   ";
		//Item 5
			cout << "  " << item11.icon;
			if(item11.quantity>1) cout << "(" <<item11.quantity << ")";
			else cout << "   ";

		//Item 6 (1 for testing..)
			cout << "  " << item12.icon;
			if(item12.quantity>1) cout << "(" <<item12.quantity << ")";
			else cout << "   ";
			
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
	if(item1.name!="") cout << item1.name << "(" << coin << item1.value << ")" << ", ";
	if(item2.name!="") cout << item2.name << "(" << coin << item2.value << ")" << ", ";
	if(item3.name!="") cout << item3.name << "(" << coin << item3.value << ")" << ", ";
	if(item4.name!="") cout << item4.name << "(" << coin << item4.value << ")" << ", ";
	if(item5.name!="") cout << item5.name << "(" << coin << item5.value << ")" << ", ";
	if(item6.name!="") cout << item6.name << "(" << coin << item6.value << ")" << ", ";
	if(item7.name!="") cout << item7.name << "(" << coin << item7.value << ")" << ", ";
	if(item8.name!="") cout << item8.name << "(" << coin << item8.value << ")" << ", ";
	if(item9.name!="") cout << item9.name << "(" << coin << item9.value << ")" << ", ";
	if(item10.name!="") cout << item10.name << "(" << coin << item10.value << ")" << ", ";
	if(item11.name!="") cout << item11.name << "(" << coin << item11.value << ")" << ", ";
	if(item12.name!="") cout << item12.name << "(" << coin << item12.value << ")" << " ";
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
	switch(spot)
	{
	case 1:
		item1=new_item;
		break;
	case 2:
		item2=new_item;
		break; 
	case 3:
		item3=new_item;
		break;
	case 4:
		item4=new_item;
		break; 
	case 5:
		item5=new_item;
		break; 
	case 6:
		item6=new_item;
		break;
	case 7:
		item7=new_item;
		break; 
	case 8:
		item8=new_item;
		break;
	case 9:
		item9=new_item;
		break; 
	case 10:
		item10=new_item;
		break; 
	case 11:
		item11=new_item;
		break; 
	case 12:
		item12=new_item;
		break; 
	};
}

//Adds item to inventory. If item already exists in inventory, it adds to the stack.
void Character::Inventory::addItem(const Items& new_item)
{
	int invSlot=getSpotOfItemInInv(new_item);
	if(invSlot>0)
	{
		switch(invSlot)
		{
			case 1:
				item1.quantity++;
				break;
			case 2:
				item2.quantity++;
				break; 
			case 3:
				item3.quantity++;
				break;
			case 4:
				item4.quantity++;
				break; 
			case 5:
				item5.quantity++;
				break; 
			case 6:
				item6.quantity++;
				break;
			case 7:
				item7.quantity++;
				break; 
			case 8:
				item8.quantity++;
				break;
			case 9:
				item9.quantity++;
				break; 
			case 10:
				item10.quantity++;
				break; 
			case 11:
				item11.quantity++;
				break; 
			case 12:
				item12.quantity++;
				break; 
		};
	}
	else if(invSlot==0)
	{
		numItemsHolding++;
		switch(numItemsHolding)
		{
			case 1:
				item1=new_item;
				break;
			case 2:
				item2=new_item;
				break; 
			case 3:
				item3=new_item;
				break;
			case 4:
				item4=new_item;
				break; 
			case 5:
				item5=new_item;
				break; 
			case 6:
				item6=new_item;
				break;
			case 7:
				item7=new_item;
				break; 
			case 8:
				item8=new_item;
				break;
			case 9:
				item9=new_item;
				break; 
			case 10:
				item10=new_item;
				break; 
			case 11:
				item11=new_item;
				break; 
			case 12:
				item12=new_item;
				break; 
		};
	}
}

//Sets all items
void Character::Inventory::setAllItems(Items& i1,Items& i2, Items& i3, Items& i4,Items& i5,Items& i6,Items& i7,Items& i8,Items& i9,Items& i10,Items& i11,Items& i12)
{
	item1=i1;
	item2=i2;
	item3=i3;
	item4=i4;
	item5=i5;
	item6=i6;
	item7=i7;
	item8=i8;
	item9=i9;
	item10=i10;
	item11=i11;
	item12=i12;
}

//Finds the location of an item in the inventory.
int Character::Inventory::getSpotOfItemInInv(const Items& new_item)
{
	char icons[12]={item1.icon,
		item2.icon,
		item3.icon,
		item4.icon,
		item6.icon,
		item7.icon,
		item8.icon,
		item9.icon,
		item10.icon,
		item11.icon,
		item12.icon};

	for(int i=0;i<12;i++)
	{
		if(icons[i]==new_item.icon)
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
				else if(input==vendor_inv.item1.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item1.value)
					{
						cout << "\n" << vendor_inv.item1.name << "bought for " << vendor_inv.item1.value << " coins!\n";
						coins=coins-vendor_inv.item1.value;
						inv.addItem(vendor_inv.item1);
						break;
					}
					else notEnough=vendor_inv.item1.name;
				}
				else if(input==vendor_inv.item2.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item2.value)
					{
						cout << "\n" << vendor_inv.item2.name << "bought for " << vendor_inv.item2.value << " coins!\n";
						coins=coins-vendor_inv.item2.value;
						inv.addItem(vendor_inv.item2);
						break;
					}
					else notEnough=vendor_inv.item2.name;
				}
				else if(input==vendor_inv.item3.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item3.value)
					{
						cout << "\n" << vendor_inv.item3.name << "bought for " << vendor_inv.item3.value << " coins!\n";
						coins=coins-vendor_inv.item3.value;
						inv.addItem(vendor_inv.item3);
						break;
					}
					else notEnough=vendor_inv.item3.name;
				}
				else if(input==vendor_inv.item4.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item4.value)
					{
						cout << "\n" << vendor_inv.item4.name << "bought for " << vendor_inv.item4.value << " coins!\n";
						coins=coins-vendor_inv.item4.value;
						inv.addItem(vendor_inv.item4);
						break;
					}
					else notEnough=vendor_inv.item4.name;
				}
				else if(input==vendor_inv.item5.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item5.value)
					{
						cout << "\n" << vendor_inv.item5.name << "bought for " << vendor_inv.item5.value << " coins!\n";
						coins=coins-vendor_inv.item5.value;
						inv.addItem(vendor_inv.item5);
						break;
					}
					else notEnough=vendor_inv.item5.name;
				}
				else if(input==vendor_inv.item6.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item6.value)
					{
						cout << "\n" << vendor_inv.item6.name << "bought for " << vendor_inv.item6.value << " coins!\n";
						coins=coins-vendor_inv.item6.value;
						inv.addItem(vendor_inv.item6);
						break;
					}
					else notEnough=vendor_inv.item6.name;
				}
				else if(input==vendor_inv.item7.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item7.value)
					{
						cout << "\n" << vendor_inv.item7.name << "bought for " << vendor_inv.item7.value << " coins!\n";
						coins=coins-vendor_inv.item7.value;
						inv.addItem(vendor_inv.item7);
						break;
					}
					else notEnough=vendor_inv.item7.name;
				}
				else if(input==vendor_inv.item8.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item8.value)
					{
						cout << "\n" << vendor_inv.item8.name << "bought for " << vendor_inv.item8.value << " coins!\n";
						coins=coins-vendor_inv.item8.value;
						inv.addItem(vendor_inv.item8);
						break;
					}
					else notEnough=vendor_inv.item8.name;
				}
				else if(input==vendor_inv.item9.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item9.value)
					{
						cout << "\n" << vendor_inv.item9.name << "bought for " << vendor_inv.item9.value << " coins!\n";
						coins=coins-vendor_inv.item9.value;
						inv.addItem(vendor_inv.item9);
						break;
					}
					else notEnough=vendor_inv.item9.name;
				}
				else if(input==vendor_inv.item10.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item10.value)
					{
						cout << "\n" << vendor_inv.item10.name << "bought for " << vendor_inv.item10.value << " coins!\n";
						coins=coins-vendor_inv.item10.value;
						inv.addItem(vendor_inv.item10);
						break;
					}
					else notEnough=vendor_inv.item10.name;
				}
				else if(input==vendor_inv.item11.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item11.value)
					{
						cout << "\n" << vendor_inv.item11.name << "bought for " << vendor_inv.item11.value << " coins!\n";
						coins=coins-vendor_inv.item11.value;
						inv.addItem(vendor_inv.item11);
						break;
					}
					else notEnough=vendor_inv.item11.name;
				}
				else if(input==vendor_inv.item12.name)
				{
					notEnough="false";
					if(coins>vendor_inv.item12.value)
					{
						cout << "\n" << vendor_inv.item12.name << "bought for " << vendor_inv.item12.value << " coins!\n";
						coins=coins-vendor_inv.item12.value;
						inv.addItem(vendor_inv.item12);
						break;
					}
					else notEnough=vendor_inv.item12.name;
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
				else if(input==inv.item1.name)
				{
					cout << "\n" << inv.item1.name << " sold for " << inv.item1.value << " coins!\n";
					coins=coins+inv.item1.value;
					inv.removeItem(1);
					break;
				}
				else if(input==inv.item2.name)
				{
					cout << "\n" << inv.item2.name << " sold for " << inv.item2.value << " coins!\n";
					coins=coins+inv.item2.value;
					inv.removeItem(2);
					break;
				}
				else if(input==inv.item3.name)
				{
					cout << "\n" << inv.item3.name << " sold for " << inv.item3.value << " coins!\n";
					coins=coins+inv.item3.value;
					inv.removeItem(3);
					break;
				}
				else if(input==inv.item4.name)
				{
					cout << "\n" << inv.item4.name << " sold for " << inv.item4.value << " coins!\n";
					coins=coins+inv.item4.value;
					inv.removeItem(4);
					break;
				}
				else if(input==inv.item5.name)
				{
					cout << "\n" << inv.item5.name << " sold for " << inv.item5.value << " coins!\n";
					coins=coins+inv.item5.value;
					inv.removeItem(5);
					break;
				}
				else if(input==inv.item6.name)
				{
					cout << "\n" << inv.item6.name << " sold for " << inv.item6.value << " coins!\n";
					coins=coins+inv.item6.value;
					inv.removeItem(6);
					break;
				}
				else if(input==inv.item7.name)
				{
					cout << "\n" << inv.item7.name << " sold for " << inv.item7.value << " coins!\n";
					coins=coins+inv.item7.value;
					inv.removeItem(7);
					break;
				}
				else if(input==inv.item8.name)
				{
					cout << "\n" << inv.item8.name << " sold for " << inv.item8.value << " coins!\n";
					coins=coins+inv.item8.value;
					inv.removeItem(8);
					break;
				}
				else if(input==inv.item9.name)
				{
					cout << "\n" << inv.item9.name << " sold for " << inv.item9.value << " coins!\n";
					coins=coins+inv.item9.value;
					inv.removeItem(9);
					break;
				}
				else if(input==inv.item10.name)
				{
					cout << "\n" << inv.item10.name << " sold for " << inv.item10.value << " coins!\n";
					coins=coins+inv.item10.value;
					inv.removeItem(10);
					break;
				}
				else if(input==inv.item11.name)
				{
					cout << "\n" << inv.item11.name << " sold for " << inv.item11.value << " coins!\n";
					coins=coins+inv.item11.value;
					inv.removeItem(11);
					break;
				}
				else if(input==inv.item12.name)
				{
					cout << "\n" << inv.item12.name << " sold for " << inv.item12.value << " coins!\n";
					coins=coins+inv.item12.value;
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
	switch(spot)
	{
	case 1:
		item1.quantity--;
		break;
	case 2:
		item2.quantity--;
		break;
	case 3:
		item3.quantity--;
		break;
	case 4:
		item4.quantity--;
		break;
	case 5:
		item5.quantity--;
		break;
	case 6:
		item6.quantity--;
		break;
	case 7:
		item7.quantity--;
		break;
	case 8:
		item8.quantity--;
		break;
	case 9:
		item9.quantity--;
		break;
	case 10:
		item10.quantity--;
		break;
	case 11:
		item11.quantity--;
		break;
	case 12:
		item12.quantity--;
		break;
	};
	if(item1.quantity==0) item1=empty;
	if(item2.quantity==0) item2=empty;
	if(item3.quantity==0) item3=empty;
	if(item4.quantity==0) item4=empty;
	if(item5.quantity==0) item5=empty;
	if(item6.quantity==0) item6=empty;
	if(item7.quantity==0) item7=empty;
	if(item8.quantity==0) item8=empty;
	if(item9.quantity==0) item9=empty;
	if(item10.quantity==0) item10=empty;
	if(item11.quantity==0) item11=empty;
	if(item12.quantity==0) item12=empty;
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