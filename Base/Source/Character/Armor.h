#ifndef ARMOR_H
#define ARMOR_H

#include "Item.h"

struct Armor : public Item
{
	Armor();
	Armor(int str, int dex, int luk, int hp, bool equipped,string name);
	~Armor();

	int i_strBoost;
	int i_dexBoost;
	int i_lukBoost;
	int i_hpBoost;
};
#endif ARMOR_H