//GameMenu.cpp
#include "GameMenu.h"

//--Project Headers--//
#include "Game.h"
#include "Maps.h"
#include "Character.h"
#include "Items.h"
#include "Settings.h"
#include "Player.h"
#include "Npc.h"

//--Language Headers--//
#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

//--Namespaces--//
using namespace std;
using namespace System;

//--Global Variables--//
#define CHOICES 5 //max menu choice

//Initialize if new game
void GameMenu::mainMenu(Settings& currentSettings)
{
	//Init Map
	Maps currentLevel(1,1,currentSettings);

	//Init Player
	Player p1(currentSettings,currentLevel);

	//Run Menu
	mainMenu(currentSettings,p1,currentLevel,false);
}

// Game Main Menu Method
void GameMenu::mainMenu(Settings& currentSettings,Player& p1,Maps& currentLevel,bool gameStarted)
{	
	//Init Window
	system("color 9f");

	//***Main Menu Loop
	bool sure=false;
	int option=0;
	printMenu(option,gameStarted);
	while(!GetAsyncKeyState(VK_RETURN))
	{
		if(GetAsyncKeyState(VK_UP) || GetAsyncKeyState(0x57))        // Move up menu
		{
			option-=1;
			if(option<0)option=CHOICES-1;
			printMenu(option,gameStarted);	
			Sleep(100);
		}
		else if(GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(0x53)) // Move down menu
		{
			option+=1;
			if(option>CHOICES-1)option=0;
			printMenu(option,gameStarted);
			Sleep(100);
		}
		else if(GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(0x44) ||  GetAsyncKeyState(VK_RETURN))  // Enter Menu Option
		{
			// First Option
			if(option==0) 
			{
				if(gameStarted==false) 
				{
					Game newGame(currentSettings,p1,currentLevel,true);
					newGame.runGame();					
				}
				else if(gameStarted==true)
				{
					load(currentSettings,p1,currentLevel,true);
				}
			}

			// Second Option
			else if(option==1)
			{
				if(gameStarted==false)
				{
					load(currentSettings,p1,currentLevel,false);
				}
				else if(gameStarted==true)
				{
					save(currentSettings,p1,currentLevel);
				}
			}

			// Third Option
			else if(option==2)
			{
				if(gameStarted==false)
				{
					Settings settingsObj(true,0,0,false,false);//edit to use obj values
					settingsObj.settingsMenu(currentSettings,p1,currentLevel);
				}
				else if(gameStarted==true)
				{
					load(currentSettings,p1,currentLevel,false);
				}
			}

			// Fourth Option
			else if(option==3)
			{
				printInstructions();
			}
			// Fifth Option
			else if(option==4)
			{
				exit(0);
			}
		}
		else if(GetAsyncKeyState(VK_ESCAPE))
		{
			if(gameStarted==false) 
			{
				exit(0);
			}
			else if(gameStarted==true)
			{
				GameMenu goBackToMainMenu;
				goBackToMainMenu.mainMenu(currentSettings,p1,currentLevel,false);
			}
		}
		else continue;
	} 
}

// Print menu method - Prints the menu from mainMenu 
void GameMenu::printMenu(int option,bool gameStarted)
{
	if(!gameStarted)
	{
		system("CLS");
		cout << "\n              ** Hello and Welcome! **\n\n     1) Use the arrows keys to navigate the menu.\n     2) Hit enter to chose an option.\n     3) Hit escape to exit.\n\n";
		for(int i=0;i<CHOICES;i++)
		{
			switch(i)
			{
				case 0:
					cout << "       New Game ";
					break;
				case 1:
					cout << "      Load Game ";
					break;
				case 2:
					cout << "       Settings ";
					break;
				case 3:
					cout << "   Instructions ";
					break;
				case 4:
					cout << "      Quit Game ";
					break;
			};
			if(i==option)
			{
				std::cout << "(*)";
			}
			else 
			{
				std::cout << "( )";
			}
			cout << "\n";
		}
	}
	else
	{	
		system("CLS");
		cout << "\n              ** GAME PAUSED **\n\n     1) Hit Escape to exit to the Main Menu(without saving).\n\n";
		for(int i=0;i<CHOICES;i++)
		{
			switch(i)
			{
				case 0:
					cout << "      Play Game ";
					break;
				case 1:
					cout << "      Save Game ";
					break;
				case 2:
					cout << "      Load Game ";
					break;
				case 3:
					cout << "   Instructions ";
					break;
				case 4:
					cout << "      Quit Game ";
					break;
			};
			if(i==option)
			{
				std::cout << "(*)";
			}
			else 
			{
				std::cout << "( )";
			}
			cout << "\n";
		}
	}
	cout << "\n\n\n      (C) Grant Griffiths 2O12";
}

// Save Method - Allows users to save a game
void GameMenu::save(Settings& currentSettings,Player& p1,Maps& currentLevel)
{
	int numFiles=0;
	string map;
	char charToString;
	bool overwrite=true;
	
	//Encrypt map and put into string format
	for(unsigned int k=0;k<22;k++)
	{
		for(unsigned int i=0;i<42;i++)
		{
			charToString=currentLevel.map[k][i];
			if     (currentLevel.map[k][i]==1) charToString = 1; // Block
			else if(currentLevel.map[k][i]==2) charToString = 2;  // Character
			else if(currentLevel.map[k][i]==3) charToString = 3;  // Point
			else if(currentLevel.map[k][i]==4) charToString = 4;  // Up
			else if(currentLevel.map[k][i]==5) charToString = 5;  // Down
			else if(currentLevel.map[k][i]==6) charToString = 6;  // Down
			else                               charToString = 9;  // Space
			map+=charToString;
		}
		if(k<21) map+="\n";
		
	}
	std::system("CLS");

	//Generate random placeholders
	char dLine[22][42];
	for(int i=0;i<22;i++)
	{
		for(int k=0;k<42;k++)
		{
			srand((unsigned int)time(NULL));
			dLine[k][i]=rand()%42;
		}
	}

	//Encrypt other properties & put into string format
	string dLineStr[22];
	dLine[0][17]=5;
	dLine[1][15]=5;
	dLine[2][12]=5;
	dLine[3][20]=1;
	dLine[4][19]=currentLevel.level;
	dLine[5][11]=currentLevel.world;
	dLine[6][12]=5;
	for(unsigned int k=0;k<7;k++)
	{
		for(unsigned int i=0;i<42;i++)
		{
			dLineStr[k]+=dLine[k][i];
			
		}
	}

	//Get save directory into array and output it 
	fstream dir("DIR.ggi");
	string line;
	string savedGames[100];
	dir >> numFiles;
	getline(dir,line);
	cout << numFiles << " saved games found.\n\n";
	for(int i=0;i<numFiles;i++)
	{
		getline(dir,savedGames[i]);
		cout << i+1 << "). " << savedGames[i] << endl;
	}
	dir.close();


	//Get File Name 	
	string saveName;
	string saveFile;
	Sleep(500);
	while(1)
	{
		cout << "\nEnter Save File Name(or X to go back): ";
		getline (cin, saveName);
		if(saveName=="")
		{
			continue;
		}
		else if(saveName=="x" || saveName == "X")
		{
			break;
		}
		else
		{
			saveFile = (saveName+".ggm");
			break;
		}
	}
	if(saveName=="x" || saveName == "X")
	{
		GameMenu goBack;
		goBack.mainMenu(currentSettings,p1,currentLevel,true);
	}
	//Check nameTaken or not
	bool nameTaken=false;
	for(int i=0;i<numFiles;i++)
	{
		if(savedGames[i]==saveName)
		{
			nameTaken=true;
		}
	}
	
	/* Ask to overwrite or not(y/n)
	** if y, override
	** if n, chose new name
	** else, ask again */
	bool updateDir=true;
	string choice="n";
	int nameOverrideCount=0;
	while(nameTaken)
	{
		cout << "\nOverwrite file " << saveFile << "? (y/n)\n";
		cin >> choice;
		if(choice=="y" || choice =="Y")
		{
			updateDir=false;
			nameTaken=false;
			break;
		}
		else if(choice=="n" || choice == "N")
		{
			break;
		}
		else continue;
	}
	Sleep(250);

	//Repeatedly prompt user to enter name until name isn't a duplcate
	while(nameTaken)
	{
		nameOverrideCount=0;
		cout << "\nERROR1: " << "The name " << saveName << " is already taken.\nPlease choose another name: ";
		getline (cin,saveName);
		for(int i=0;i<numFiles;i++)
		{
			if(savedGames[i]==saveName)
			{
				nameOverrideCount++;
			}
		}
		if(nameOverrideCount>0)
		{
			continue;
		}
		else break;
	}

	/* If new DIR value is need:
	** Increment number of files
	** Add saveName to dir
	** output to dir */
	if(updateDir)
	{
		numFiles++;
		savedGames[numFiles-1]=saveName;
		fstream newDir("DIR.ggi");
		newDir << numFiles << "\n";
		for(int i=0;i<numFiles;i++)
		{
			newDir << savedGames[i] << "\n";
		}
		newDir.close();
	}
	

	//Output properties to saveGame file
	saveFile = (saveName+".ggm");
	ofstream saveGame(saveFile);
	saveGame << map << "\n";
	saveGame << dLineStr[0] << "\n";
	saveGame << dLineStr[1] << "\n";
	saveGame << dLineStr[2] << "\n";
	saveGame << dLineStr[3]<< "\n";
    saveGame << dLineStr[4] << "\n";
	saveGame << dLineStr[5] << "\n";
	saveGame << dLineStr[6] << "\n";
	saveGame.close();

	//Prompt user to continue
	cout << "\nGame has been saved under name: " << saveFile << ".";
	Sleep(1000);
}

// Load Method - Allows users to load old games
void GameMenu::load(Settings& currentSettings,Player& p1,Maps& currentLevel, bool skipToLoad)
{
	int filesFound=0;
	std::system("CLS");
	int x=0,y=0,coins=0,level=0,world=0,health=0;
	char loadedMap[22][42];
	int numFiles=NULL;

	//Skip to loading
	if(skipToLoad)
	{
		Game continueGame(currentSettings,p1,currentLevel,false);
		Maps currentLevel(currentLevel.level,currentLevel.world,currentSettings); 
		//currentLevel.map[p1.y][p1.x]=2;
	
		//Load Game *NEW game
		continueGame.runGame();
	}

	//Put save directory into array and output it 
	fstream dir("DIR.ggi");
	string line=" ";
	string savedGames[100];
	dir >> numFiles;
	getline(dir,line);
	cout << "\n  " << numFiles << " saved game(s) found.\n\n";
	for(int i=0;i<numFiles;i++)
	{
		getline(dir,savedGames[i]);
		cout << "     " << i+1 << "). " << savedGames[i] << endl;
	}
	dir.close();

	//Ask for file name & Check if file exists
	string loadFile=" ";
	string loadName=" ";
	
	Sleep(500);
	while(filesFound!=1)
	{
		cout << "\n  Enter Load File Name(or X to go back): ";
		getline (cin, loadName);
		if(loadName=="x" || loadName == "X")
		{
			break;
		}
		loadFile=loadName+".ggm";
	
		for(int i=0;i<numFiles;i++)
		{
			if((savedGames[i]+".ggm")==loadFile)
			{
				filesFound++;
			}			
		}
		if(filesFound==0) cout << "  ERROR: Could not find file " << loadFile << "\n";
		else break;
	}
	if(loadName=="x" || loadName == "X")
	{
		GameMenu goBack;
		goBack.mainMenu(currentSettings,p1,currentLevel,true);
	}

	//Load file properties & Decrypt them
	int c=0;
	string strmap;
	ifstream load(loadFile);
	for(int k=0;k<22;k++) 
	{
		getline(load,line);
		strmap+=line;
		for(int d=0;d<42;d++)
		{
			if     (strmap.at(c)==1)  loadedMap[k][d] = 1;  // Wall
			else if(strmap.at(c)==3)  loadedMap[k][d] = 3;  // Coin
			else if(strmap.at(c)==4)  loadedMap[k][d] = 4;  // Up
			else if(strmap.at(c)==5)  loadedMap[k][d] = 5;  // Down
			else if(strmap.at(c)==6)  loadedMap[k][d] = 6;  // Door
			else if(strmap.at(c)==9)  loadedMap[k][d] = 9;  // Space
			c++;
		}
	}
	getline(load,line);
		x=line.at(17);
	getline(load,line);
		y=line.at(15);
	getline(load,line);
		coins=line.at(12);
	getline(load,line);
		level=line.at(19);
	getline(load,line);
		world=line.at(11);
	getline(load,line);
		health=line.at(12);

	//Load Inventory... change me
	Items loadedItem1(0);
	Items loadedItem2(0);
	Items loadedItem3(0);
	Items loadedItem4(0);
	Items loadedItem5(0);
	int loadedItemNum=0;

	//Prompt user to continue
	cout << "\nGame is being loaded under name: " << loadFile << endl;

	//Create new objects for game & set Character coords
	Player loadedP1(health,x,y,coins,0,0x21,false,currentSettings);
	Maps currentLevel(loadedMap,level,world,currentSettings,&loadedP1); 
	//currentLevel.map[loadedP1.y][loadedP1.x]=2;
	Sleep(2000);
	system("pause");


	//Load Game
	Game loadedGame(currentSettings,loadedP1,currentLevel,false);
	loadedGame.runGame();
}

// Prints instruction to the screen
void GameMenu::printInstructions()
{
	system("CLS");
	cout << "\n  ** Instructions **";
	cout << "\n        1.) Move with WASD or the arrow keys.";
	cout << "\n        2.) Interact with E";
	cout << "\n        3.) Attack with C";
	cout << "\n        4.) View inventory with TAB";

	cout << "\n";
	Sleep(2000);
	system("pause");
}