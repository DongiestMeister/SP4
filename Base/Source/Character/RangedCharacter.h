#ifndef RANGEDCHARACTER_H
#define RANGEDCHARACTER_H

#include "Character.h"
class RangedCharacter : public Character
{
public:
	RangedCharacter(string name);
	virtual ~RangedCharacter();
	virtual bool attack(Character* opponent);
	virtual RangedCharacter* clone();
};

#endif RANGEDCHARACTER_H