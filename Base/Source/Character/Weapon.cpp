#include "Weapon.h"


Weapon::Weapon() : i_damageValue(0), b_isEquippedToSomeone(false)
{
}

// Takes in an int value to use as the weapon's damage
Weapon::Weapon(int damage,int weaponAcc, bool equip)
{
	i_damageValue = damage;
	i_weaponAccuracy = weaponAcc;
	b_isEquippedToSomeone = equip;
}

Weapon::~Weapon()
{
}
