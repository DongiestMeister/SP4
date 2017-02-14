#ifndef WEAPON_H
#define WEAPON_H

struct Weapon
{
	Weapon();
	Weapon(int damage,int weaponAcc, bool equip);
	~Weapon();
	void Use();

	bool b_isEquippedToSomeone;
	int i_damageValue;
	int i_weaponAccuracy;
};
#endif WEAPON_H

