#ifndef WEAPON_H
#define WEAPON_H

#include <string>

using std::string;

struct Weapon
{
	Weapon();
	Weapon(int damage,int weaponAcc, bool equip,string name);
	~Weapon();
	void Use();

	bool b_isEquippedToSomeone;
	int i_damageValue;
	int i_weaponAccuracy;

	string name;
};
#endif WEAPON_H

