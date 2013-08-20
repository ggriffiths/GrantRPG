#pragma once
#include "Items.h"
#include <string>
#include <vector>

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
		Items bag[12];

		// Change Inventory
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
