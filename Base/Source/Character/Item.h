#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "Mesh.h"

using std::string;

struct Item // All items to inherit from this class
{
	string s_ownerName; // who is equipping this item. should be "" if its a usable item, or if the equipment is not equipped to anyone.
	string s_Name;
	bool b_isEquippedToSomeone;
	Mesh* itemPortrait;

	Item() : s_ownerName(""){ itemPortrait = nullptr; }
	virtual ~Item(){ if (itemPortrait){ delete itemPortrait; itemPortrait = nullptr; } }

};

#endif ITEM_H