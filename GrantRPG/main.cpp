//main.cpp

//--Project Headers--//
#include "GameMenu.h"
#include "Maps.h"
#include "Character.h"
#include "Settings.h"

//--Language Headers--//
#include <ctime>
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>

//--Namespaces--//
using namespace std;
using namespace System;

void console_initialize()
{
	ShowWindow(GetConsoleWindow(),SW_MAXIMIZE);
	SetConsoleTitle("Grant's Game");
}


//Main Function
int main()
{	
	//Initialize Objects
	GameMenu menuObj;
	Settings loadedSettings(true,0,0,false,false);
	
	//Initialization
	console_initialize();

	//Load Menu
	while(1) menuObj.mainMenu(loadedSettings);
	return 0;
}

