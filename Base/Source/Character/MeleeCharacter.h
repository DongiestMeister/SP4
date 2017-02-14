#ifndef MELEE_CHARACTER_H
#define MELEE_CHARACTER_H

#include "Character.h"

class MeleeCharacter : public Character
{
public:
	MeleeCharacter();
	virtual ~MeleeCharacter();
	virtual void takeDamage(int dmg);
	virtual bool attack();
	void equipWeapon(Weapon* newWeapon);
	void equipArmor(Armor* newArmor);
	void calculateStats();
private:

};

#endif MELEE_CHARACTER_H