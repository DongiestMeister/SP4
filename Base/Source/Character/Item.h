#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "Mesh.h"

using std::string;

struct Item // All items to inherit from this class
{
	string s_ownerName; // who is equipping this item. should be "" if its a usable item, or if the equipment is not equipped to anyone.
	string s_Name;
	string itemPortrait; // name of the portrait in meshbuilder
	bool b_isEquippedToSomeone;
	bool b_isWeapon;
	bool b_isArmor;

	int i_strBoost;
	int i_dexBoost;
	int i_lukBoost;
	int i_hpBoost;

	int i_damageValue;
	int i_weaponAccuracy;

	Item() : s_ownerName(""), itemPortrait("") {}
	virtual ~Item(){}
};

#endif ITEM_H