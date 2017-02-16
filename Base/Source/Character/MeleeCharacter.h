#ifndef MELEE_CHARACTER_H
#define MELEE_CHARACTER_H

#include "Character.h"

class MeleeCharacter : public Character
{
public:
	MeleeCharacter(string name);
	virtual ~MeleeCharacter();
	virtual bool attack(Character* opponent);
};

#endif 