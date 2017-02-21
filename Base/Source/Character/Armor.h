#ifndef ARMOR_H
#define ARMOR_H

#include "Item.h"

struct Armor : public Item
{
	Armor();
	Armor(int str, int dex, int luk, int hp, bool equipped,string name);
	~Armor();

	
};
#endif ARMOR_H