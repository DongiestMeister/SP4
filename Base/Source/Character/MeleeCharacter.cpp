#include "MeleeCharacter.h"
#include <iostream>

MeleeCharacter::MeleeCharacter(string name)
{
	s_Name = name;
	i_baseSTR = 10; // All melee characters have a base STR of 10
	i_baseDEX = 3; // All melee characters have a base DEX of 3
	i_baseLUK = 0; // All melee characters have a base LUK of 0
	i_movementCost = 6; // All melee characters have a base movement cost of 3
	i_attackRange = 1; // All melee characters have a base attack range of 1

	// Damage value of 0, so equivalent to not having a weapon at all
	weapon = nullptr;
	// All stats boosts of 0, equivalent to not having an armor
	armor = nullptr;
}

MeleeCharacter::~MeleeCharacter()
{
}

bool MeleeCharacter::attack(Character* opponent)
{
	int hitRate =0;
	if (weapon != nullptr)
	{
		hitRate = (0.1 * i_DEX) + weapon->i_weaponAccuracy - (0.1 * opponent->getLUK());
	}
	else
	{
		hitRate = 0.1 * i_DEX - (opponent->getLUK() * 0.1) + 70;
	}
	
	int hitResult = Math::RandIntMinMax(0, 100);
	if (hitRate >= hitResult)
	{
		opponent->takeDamage(i_Damage);
		b_tookAction = true;
		return true;
	}
	else if (hitRate < hitResult)
	{
		b_tookAction = true;
		return false;
	}
	return false;
}

MeleeCharacter* MeleeCharacter::clone()
{
	return new MeleeCharacter(*this);
}