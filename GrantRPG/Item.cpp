//Item.cpp
#include "Item.h"

//--Project Headers--//
#include "GameMenu.h"
#include "Maps.h"
#include "Character.h"
#include "Settings.h"

//--Language Headers--//
#include <iostream>
#include <GL\glut.h>
#include <string>
#include <fstream>

//--Namespaces--//
using namespace std;


//Creates an item based on the ID given
Item::Item(int id)
{
	switch(id)
	{
		case 0:  // Empty Space
			name="";
			icon = 255;
			color=0x01;
			quantity=0;
			value=0;
			break;
		case 1:  // Sword
			name="Sword";
			icon = 159;
			color=0x0c;
			quantity=1;
			value=10;
			break;
		case 2:  // Shield
			name="Shield";
			icon = 233;
			color=0x0c;
			quantity=1;
			value=20;
			break;
	} 
}
Item::Item(){}