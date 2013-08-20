#pragma once
#include <string>
#include "windows.h"

class Item
{
public:
	Item(int id);
	Item();
	//Item Properties
	std::string name;
	int quantity;
	char icon;
	WORD color;
	int value;
};

