#ifndef ARMOR_H
#define ARMOR_H

struct Armor
{
	Armor();
	Armor(int str, int dex, int luk, int hp, bool equipped);
	~Armor();

	bool b_isEquippedToSomeone;
	int i_strBoost;
	int i_dexBoost;
	int i_lukBoost;
	int i_hpBoost;

};
#endif ARMOR_H