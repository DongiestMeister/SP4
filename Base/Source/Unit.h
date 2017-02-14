#ifndef UNIT_H
#define UNIT_H

#include "Character\Character.h"
#include "Vector2.h"

class Unit
{
public:
	Unit() { pos.Set(-1, -1); character = nullptr; }
	Unit(Character *character) {this->character = character;}
	~Unit() { if (character) delete character; character = nullptr; }
	Character *character;
	Vector2 pos;
};

#endif