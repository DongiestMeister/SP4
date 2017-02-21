#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h"

struct Weapon : public Item
{
	Weapon();
	Weapon(int damage,int weaponAcc, bool equip,string name);
	~Weapon();
};
#endif WEAPON_H

