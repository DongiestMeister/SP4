#ifndef FLYINGCHARACTER_H
#define FLYINGCHARACTER_H

#include "Character.h"
class FlyingCharacter : public Character
{
	FlyingCharacter();
	virtual ~FlyingCharacter();
	virtual bool attack(Character* opponent);
};

#endif FLYINGCHARACTER_H