#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "Mesh.h"

using std::string;

struct Item // All items to inherit from this class
{
	string s_ownerName = ""; // who is equipping this item. should be "" if its a usable item, or if the equipment is not equipped to anyone.
	string s_Name = "";
	string itemPortrait = ""; // name of the portrait in meshbuilder
	bool b_isEquippedToSomeone = false;
	bool b_isWeapon = false;
	bool b_isArmor = false;

	int i_strBoost = -1;
	int i_dexBoost = -1;
	int i_lukBoost = -1;
	int i_hpBoost = -1;

	int i_damageValue = -1;
	int i_weaponAccuracy = -1;

	int i_Price = -1; // Price of this item at the shop

	Item(){}
	virtual ~Item(){}
};

#endif ITEM_H