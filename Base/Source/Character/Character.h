#ifndef CHARACTER_H
#define CHARACTER_H

#include "Weapon.h"
#include "MyMath.h"

class Character
{
public:
	Character(){i_HP = 100;} // Hp always default to 100
	virtual ~Character(){};
	virtual void takeDamage(int dmg) = 0; // returns true if damage was taken (if the opposing attack did not miss)
	virtual bool attack() = 0; // returns true if the attack was successful
	//virtual void moveCharacter() = 0;
	int i_movementCost;
	int i_stepsTaken;
protected:
	Weapon* weapon;
	//Armor* armor;
	int i_HP;
	int i_Damage;
	// Stats
	int i_STR; // Determines the unit's attack power
	int i_DEX; // Determines the accuracy of the attacks of this unit
	int i_LUK; // Determines the avoidability of this unit
};

#endif CHARACTER_H
