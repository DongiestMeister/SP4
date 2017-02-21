#include "Character.h"

Character::Character()
{
	i_HP = 100;
	i_currentHP = 100;
	i_idInParty = -1;
	character3DMesh = nullptr;
	character2DMesh = nullptr;
	characterPortrait = nullptr;
	weapon = nullptr;
	armor = nullptr;

	b_tookAction = false;
	i_stepsTaken = 0;
	b_inParty = false;
	s_Name = "";

	FSM = nullptr;

	i_strBoostFromTerrain = 0;
	i_dexBoostFromTerrain = 0;
	i_lukBoostFromTerrain = 0;

	i_Damage = 0;

	strategy = DEFENCE;

}

Character::~Character()
{
	if (weapon)
	{
		//delete weapon;
		//weapon = nullptr;
	}
	if (armor)
	{
		//delete armor;
		//armor = nullptr;
	}
	if (character3DMesh)
	{
		//delete character3DMesh;
		//character3DMesh = nullptr;
	}
	if (character2DMesh)
	{
		//delete character2DMesh;
		//character2DMesh = nullptr;
	}
	if (characterPortrait)
	{
		//delete characterPortrait;
		//characterPortrait = nullptr;
	}
	if (FSM)
	{
		delete FSM;
		FSM = nullptr;
	}
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

Mesh* Character::get3DMesh()
{
	if (character3DMesh)
		return character3DMesh;
	return nullptr;
}

Mesh* Character::get2DMesh()
{
	if (character2DMesh)
		return character2DMesh;
	return nullptr;
}

Mesh* Character::getPortrait()
{
	if (characterPortrait)
		return characterPortrait;
	return nullptr;
}

Weapon* Character::getWeapon()
{
	if (weapon)
		return weapon;
	return nullptr;
}

Armor* Character::getArmor()
{
	if (armor)
		return armor;
	return nullptr;
}

string Character::getName()
{
	return s_Name;
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

void Character::set3DMesh(Mesh* newMesh)
{
	character3DMesh = newMesh;
}

void Character::set2DMesh(Mesh* newMesh)
{
	character2DMesh = newMesh;
}

void Character::setPortrait(Mesh* newMesh)
{
	characterPortrait = newMesh;
}

void Character::takeDamage(int dmg)
{
	i_HP -= dmg;
}

void Character::calculateStats()
{
	if (armor)
	{
		i_HP += armor->i_hpBoost;
		i_STR += armor->i_strBoost + i_strBoostFromTerrain;
		i_DEX += armor->i_dexBoost + i_dexBoostFromTerrain;
		i_LUK += armor->i_lukBoost + i_lukBoostFromTerrain;
	}
	if (weapon)
	{
		i_Damage = ((0.2 * i_STR) + (0.3 * (weapon->i_damageValue))) + 2;
	}
	else
	{
		i_Damage = (0.2 * i_STR) + 2;
	}
}

void Character::equipWeapon(Weapon* newWeapon)
{
	if (newWeapon->b_isEquippedToSomeone == false)
	{
		if (weapon)
		{
			weapon->s_ownerName = "";
			weapon->b_isEquippedToSomeone = false;
		}
		weapon = newWeapon;
		weapon->s_ownerName = s_Name;
		weapon->b_isEquippedToSomeone = true;
		calculateStats();
	}
}

void Character::equipArmor(Armor* newArmor)
{
	if (newArmor->b_isEquippedToSomeone == false)
	{
		if (armor)
		{
			armor->s_ownerName = "";
			armor->b_isEquippedToSomeone = false;
		}
		armor = newArmor;
		armor->s_ownerName = s_Name;
		armor->b_isEquippedToSomeone = true;
		calculateStats();
	}
}