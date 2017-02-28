#include "RangedCharacter.h"
#include <iostream>
RangedCharacter::RangedCharacter(string name)
{
	s_Name = name;
	i_baseSTR = 3; // All ranged characters have a base STR of 10
	i_baseDEX = 10; // All ranged characters have a base DEX of 3
	i_baseLUK = 5; // All ranged characters have a base LUK of 0
	i_movementCost = 6; // All ranged characters have a base movement cost of 3
	i_attackRange = 2; // All ranged characters have a base attack range of 2

	// Damage value of 0, so equivalent to not having a weapon at all
	weapon = nullptr;

	// All stats boosts of 0, equivalent to not having an armor
	armor = nullptr;
}

RangedCharacter::~RangedCharacter()
{
}

bool RangedCharacter::attack(Character* opponent)
{
	float distanceToEnemy = 0;
	int hitRate = 0;
	if (!(pos - opponent->getPos()).IsZero())
	{
		distanceToEnemy = (pos - opponent->getPos()).Length();
	}

	if (weapon != nullptr)
	{
		hitRate = (i_DEX + weapon->i_weaponAccuracy - opponent->getLUK()) - (int)(distanceToEnemy);
	}
	else
	{
		 hitRate = (i_DEX - opponent->getLUK()) - (int)distanceToEnemy;
	}
	int hitResult = Math::RandIntMinMax(0, 100);
	if (hitRate >= hitResult)
	{
		opponent->takeDamage(i_Damage);
		std::cout << "att went tru" << std::endl;
		b_tookAction = true;
		return true;
	}
	if (hitRate < hitResult)
	{
		std::cout << "attack failed" << std::endl;
		b_tookAction = true;
		return false;
	}
	return false;
}

RangedCharacter* RangedCharacter::clone()
{
	return new RangedCharacter(*this);
}