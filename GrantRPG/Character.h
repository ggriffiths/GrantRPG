#pragma once
#include "Items.h"
#include <string>

class Maps;
class Npc;
class Settings;
class Player;

class Character
{
	public:
	class Inventory
	{
	public:
		Inventory(bool loadFromFile);
		// Inventory Slots
		int numItemsHolding;
		Items item1;
		Items item2;
		Items item3;
		Items item4;
		Items item5;
		Items item6;
		Items item7;
		Items item8;
		Items item9;
		Items item10;
		Items item11;
		Items item12;

		// Change Inventory
		void setAllItems(Items& i1,Items& i2, Items& i3, Items& i4,Items& i5,Items& i6,Items& i7,Items& i8,Items& i9,Items& i10,Items& i11,Items& i12);
		void setItem(Items& new_item,int spot);
		void addItem(const Items& new_item); 
		void removeItem(int spot);

		// Inventory Detection
		int findItemSpot(const Items& check_item);
		int getSpotOfItemInInv(const Items& new_item);

		//Inventory Utilities
		void printInventory(bool player);
		void pSpaces(int size);
	};
	//ctor
	Character(int new_health,int new_x,int new_y,int new_coins,char new_icon,WORD new_color,bool new_harm,Settings& currentSettingsw);
	Character(Settings& currentSettings,Maps& currentLevel);
	Character();

	// Character Handling
	void handlePlayerMovement(int key,Maps* currentLevel);
	void handlePlayerAction(int key,Maps* currentLevel);

	// Public character utility
	void setLocation(int k,int i);
	void showInv();

	//Friendships
	friend class Maps;
	friend class Npc;

	//Character Properties
	char character_type;
	std::string name;
	int health;
	int x;
	int y;
	int coins;
	int attackDmg;
	int skill;
	char icon;
	bool harm;
	bool alive;
	WORD color;
	Inventory inv;

	//Character utilities
	void vendorScreen(Inventory& vendor_inv);
	char getType();

	//Character detections
	bool okayToMove(int dir,char map[22][42]);
	bool checkMapValue(int value,int dir,char map[22][42]);

	//Character Actions
	void doAction(Maps* currentLevel);
	virtual void attack(Character* enemy,Maps* currentLevel);
	void move(int dir,Maps* currentLevel,bool temp);
	void addCoin();
	void unlockDoor(int dirUnlockedFrom,Maps* currentLevel);
	bool adjacent(Character& p1,Maps& currentLevel);
};
