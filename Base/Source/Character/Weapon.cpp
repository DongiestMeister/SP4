#include "Weapon.h"


Weapon::Weapon()
{
	b_isEquippedToSomeone = false;
	s_Name = "";
	b_isWeapon = true;
	b_isArmor = false;
	i_damageValue = -1;
	i_weaponAccuracy = -1;
}

// Takes in an int value to use as the weapon's damage
Weapon::Weapon(int damage,int weaponAcc, bool equip,string name)
{
	i_damageValue = damage;
	i_weaponAccuracy = weaponAcc;
	b_isEquippedToSomeone = equip;
	s_Name = name;
}

Weapon::~Weapon()
{
}
