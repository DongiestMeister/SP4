#ifndef RANGEDCHARACTER_H
#define RANGEDCHARACTER_H

#include "Character.h"
class RangedCharacter : public Character
{
public:
	RangedCharacter();
	virtual ~RangedCharacter();
	virtual bool attack(Character* opponent);
};

#endif RANGEDCHARACTER_H