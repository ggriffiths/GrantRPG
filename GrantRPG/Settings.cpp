//Settings.cpp
#include "Settings.h"

//--Project Headers--//
#include "Game.h"
#include "GameMenu.h"
#include "Maps.h"
#include "Character.h"
#include "Items.h"

//--Language Headers--//
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

//--Namespaces--//
using namespace std;
using namespace System;

//--Global Variables--//
#define NUM_OF_SETTINGS 4

//Settings ctor
Settings::Settings(bool loadFromFile,int s1,int s2,bool s3,bool s4)
{
	if(loadFromFile)
	{
		//Declare variables and files
		string tempStr;
		char *tempChar=new char[256];
		string setting[NUM_OF_SETTINGS];
		int value[NUM_OF_SETTINGS];
		ifstream settingsFile("settings.cfg");

		//Get Settings from file
		if (settingsFile.is_open())
		{
			for(int i=0;i<NUM_OF_SETTINGS;i++)
			{
				//Get File Input
				getline(settingsFile,setting[i]);
				getline(settingsFile,tempStr);

				//Convert string value to char
				tempChar[tempStr.size()]=0;
				memcpy(tempChar,tempStr.c_str(),tempStr.size());
			
				//Convert char to to int and put into values[]
				value[i]=atoi(tempChar);
			}
		}
		else cout << "Unable to open settings. Restart game"; 

		// Set bounds, set value
		playerIcon=value[0];
		if(value[0]>255) value[0]=255; 
		if(value[0]<1)  value[0]=1;

		// Set bounds, set value
		if(value[1]>10) value[1]=10; 
		if(value[1]<0)  value[1]=1;
		moveSpeed=value[1];

		// Set bounds, set value
		if(value[2]>0) musOn=true;
		if(value[2]<0) musOn=false;

		// Set bounds, set value
		if(value[3]>0) saOn=true;
		if(value[3]<0) saOn=false;
	}
	else
	{
		// initialize these settings given from the program
		playerIcon=s1;
		if(s2>10) s2=10;
		if(s2<0) s2=1;
		moveSpeed=s2;
		musOn=s3;
		saOn=s4;
	}
}

//Decides what happens in menu
void Settings::settingsMenu(Settings& currentSettings,Player& p1,Maps& currentLevel)
{
	//Declare variables and files
	string tempStr;
	char *tempChar=new char[256];
	int option=0;
	string setting[NUM_OF_SETTINGS];
	int value[NUM_OF_SETTINGS];
	ifstream settingsFile("settings.cfg");

	//Get Settings from file
	if (settingsFile.is_open())
	{
		for(int i=0;i<NUM_OF_SETTINGS;i++)
		{
			//Get File Input
			getline(settingsFile,setting[i]);
			getline(settingsFile,tempStr);

			//Convert string value to char
			tempChar[tempStr.size()]=0;
			memcpy(tempChar,tempStr.c_str(),tempStr.size());
			
			//Convert char to to int and put into values[]
			value[i]=atoi(tempChar);
		}
	}
	else cout << "Unable to open settings. Restart game"; 
	
	//***Main Settings Menu Loop
	bool sure=false;
	bool isRunning=true;
	printSettings(setting,value,option);
	Sleep(500); //delay to avoid unintentional user input
	while(isRunning)
	{
		if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))        // Move up menu
		{
			option-=1;
			if(option<0)option=NUM_OF_SETTINGS-1;
			printSettings(setting,value,option);	
			Sleep(100);
		}
		if(GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) // Move down menu
		{
			option+=1;
			if(option>NUM_OF_SETTINGS-1)option=0;
			printSettings(setting,value,option);
			Sleep(100);
		}
		if(GetAsyncKeyState(VK_ESCAPE)) 
		{
			ofstream saveSettings("Settings.cfg");
			//Esc, save settings to file, run menu
			for(int i=0;i<NUM_OF_SETTINGS;i++)
			{
				saveSettings << setting[i] << "\n";
				saveSettings << value[i] << "\n";
			}
			settingsFile.close();
			saveSettings.close();
			GameMenu goBackToMainMenu;
			Settings newSettings(false,value[0],value[1],value[2],value[3]);
			goBackToMainMenu.mainMenu(newSettings,p1,currentLevel,false);
		}
		if(GetAsyncKeyState(VK_RETURN)) // Edit Option
		{
			cout << "\n        Change value of " << setting[option] << " to: ";
			cin >> value[option];
			printSettings(setting,value,option);
			while(GetAsyncKeyState(VK_RETURN));
		}
	}
	settingsFile.close();
}

// Prints setting to the screen
void Settings::printSettings(std::string setting[NUM_OF_SETTINGS], int value[NUM_OF_SETTINGS], int option)
{
	system("CLS");
	cout << "\n  What setting would you like to change?\n";
	cout << "\n     -> Hit ENTER to EDIT a setting.";
	cout << "\n     -> Hit ESCAPE to SAVE settings and go BACK to the MAIN MENU.\n\n"; 
	for(int i=0;i<NUM_OF_SETTINGS;i++)
	{
		if(i+1>9) cout << "     > ";
		else     cout << "     >  ";
		cout << i+1 << "). " << setting[i];
		printSpacer(35-setting[i].length());
		cout << " > " << value[i] << " < ";
		if     (value[i]>99999) printSpacer(1);
		else if(value[i]>9999)  printSpacer(2);
		else if(value[i]>999)   printSpacer(3);
		else if(value[i]>99)    printSpacer(4);
		else if(value[i]>9)     printSpacer(5);
		else printSpacer(6);
		cout << " | ";
		if(i==option)
		{
			std::cout << "x";
		}
		cout << "\n";
	}
	
}

//Prints (int) number of spaces
void Settings::printSpacer(int spaces)
{
	for(int i=0;i<spaces;i++) cout << "-";
}