#include "Character.h"

Character::Character()
{
	i_HP = 100;
	characterMesh = nullptr;
	weapon = nullptr;
	armor = nullptr;
	b_tookAction = false;
	i_stepsTaken = 0;
}

int Character::getSTR()
{
	return i_STR;
}

int Character::getDEX()
{
	return i_DEX;
}

int Character::getLUK()
{
	return i_LUK;
}

int Character::getDamage()
{
	return i_Damage;
}

int Character::getHP()
{
	return i_HP;
}

Vector2 Character::getPos()
{
	return pos;
}

Mesh* Character::getMesh()
{
	return characterMesh;
}

void Character::setSTR(int newSTR)
{
	i_STR = newSTR;
}

void Character::setDEX(int newDEX)
{
	i_DEX = newDEX;
}

void Character::setLUK(int newLUK)
{
	i_LUK = newLUK;
}

void Character::setDamage(int newDamage)
{
	i_Damage = newDamage;
}

void Character::setHP(int newHP)
{
	i_HP = newHP;
}

void Character::setPos(Vector2 newPos)
{
	pos = newPos;
}

void Character::setMesh(Mesh* newMesh)
{
	characterMesh = newMesh;
}

void Character::takeDamage(int dmg)
{
	i_HP -= dmg;
}

void Character::calculateStats()
{
	i_HP += armor->i_hpBoost;
	i_STR += armor->i_strBoost;
	i_DEX += armor->i_dexBoost;
	i_LUK += armor->i_lukBoost;
	i_Damage = ((0.2 * i_STR) + (0.3 * (weapon->i_damageValue))) + 2;
}

void Character::equipWeapon(Weapon* newWeapon)
{
	if (newWeapon->b_isEquippedToSomeone == false)
	{
		weapon->b_isEquippedToSomeone = false;
		weapon = newWeapon;
		weapon->b_isEquippedToSomeone = true;
		calculateStats();
	}
}

void Character::equipArmor(Armor* newArmor)
{
	if (newArmor->b_isEquippedToSomeone == false)
	{
		armor->b_isEquippedToSomeone = false;
		armor = newArmor;
		armor->b_isEquippedToSomeone = true;
		calculateStats();
	}
}