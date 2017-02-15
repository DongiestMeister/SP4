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

void Character::setMesh(Mesh* newMesh)
{
	characterMesh = newMesh;
}
void Character::takeDamage(int dmg)
{
	i_HP -= dmg;
}
