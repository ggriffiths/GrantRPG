#pragma once
#using <mscorlib.dll>
#include <string>
#include "windows.h"
using namespace System;

class Items
{
public:
	Items(int id);
	//Item Properties
	std::string name;
	int quantity;
	char icon;
	WORD color;
	int value;
};

