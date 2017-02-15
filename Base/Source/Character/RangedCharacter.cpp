#include "RangedCharacter.h"
#include <iostream>
RangedCharacter::RangedCharacter()
{
	i_STR = 10; // All melee characters have a base STR of 10
	i_DEX = 3; // All melee characters have a base DEX of 3
	i_LUK = 0; // All melee characters have a base LUK of 0
	i_movementCost = 3; // All melee characters have a base movement cost of 3

	// Damage value of 0, so equivalent to not having a weapon at all
	weapon = new Weapon(0, 100, true, "nothing");

	// All stats boosts of 0, equivalent to not having an armor
	armor = new Armor(0, 0, 0, 0, true);
	calculateStats();
}

RangedCharacter::~RangedCharacter()
{
	if (weapon)
	{
		delete weapon;
		weapon = nullptr;
	}
	if (armor)
	{
		delete armor;
		armor = nullptr;
	}
	if (characterMesh)
	{
		delete characterMesh;
		characterMesh = nullptr;
	}
}

bool RangedCharacter::attack(Character* opponent)
{
	int hitRate = i_DEX + weapon->i_weaponAccuracy - opponent->getLUK();
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