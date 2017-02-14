#include "MeleeCharacter.h"
#include <iostream>

MeleeCharacter::MeleeCharacter()
{
	i_STR = 10; // All melee characters have a base STR of 10
	i_DEX = 3; // All melee characters have a base DEX of 3
	i_LUK = 0; // All melee characters have a base LUK of 0
	i_movementCost = 4; // All melee characters have a base movement cost of 3
	// Damage value of 0, so equavalent to not having a weapon at all
	weapon = new Weapon(0,100, true,"Weapon");
	calculateStats();
}

MeleeCharacter::~MeleeCharacter()
{
	if (weapon)
	{
		delete weapon;
		weapon = nullptr;
	}
}

void MeleeCharacter::calculateStats()
{
	i_STR += 0;//armor->i_strBoost;
	i_DEX += 0;//armor->i_dexBoost;
	i_LUK += 0;//armor->i_lukBoost;
	i_HP += 0;//armor->i_hpBoost;
	i_Damage = ((0.2 * i_STR) + (0.3 * (weapon->i_damageValue))) + 2;
}

bool MeleeCharacter::attack(/*Enemy* enemy*/)
{
	int hitRate = i_DEX + weapon->i_weaponAccuracy; //- enemy->i_LUK
	int hitResult = Math::RandIntMinMax(0, 100);
	if (hitRate >= hitResult)
	{
		// enemy->takeDamage(i_Damage)
		std::cout << "att went tru" << std::endl;
		return true;
	}
	if (hitRate < hitResult)
	{
		std::cout << "attack failed" << std::endl;
		return false;
	}
	return false;
}

void MeleeCharacter::takeDamage(int dmg)
{
	i_HP -= dmg;
}

void MeleeCharacter::equipWeapon(Weapon* newWeapon)
{
	if (newWeapon->b_isEquippedToSomeone == false)
	{
		weapon->b_isEquippedToSomeone = false;
		weapon = newWeapon;
		weapon->b_isEquippedToSomeone = true;
		calculateStats();
	}
}