#ifndef ARMOR_H
#define ARMOR_H

#include <string>
using std::string;

struct Armor
{
	Armor();
	Armor(int str, int dex, int luk, int hp, bool equipped,string name);
	~Armor();

	bool b_isEquippedToSomeone;
	int i_strBoost;
	int i_dexBoost;
	int i_lukBoost;
	int i_hpBoost;
	string s_Name;

};
#endif ARMOR_H