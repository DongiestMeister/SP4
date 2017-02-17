#ifndef DAMAGETEXT_H
#define DAMAGETEXT_H


#include "TextEntity.h"

class DamageText
{
public:
	DamageText();
	~DamageText();
	TextEntity* dmgTxt;

	bool Update(double dt);

	
private:



};



#endif