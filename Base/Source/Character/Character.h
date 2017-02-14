#ifndef CHARACTER_H
#define CHARACTER_H

#include "Weapon.h"
#include "Armor.h"
#include "MyMath.h"
#include "Mesh.h"

class Character
{
public:
	Character(); // Hp always default to 100
	virtual ~Character(){};
	void takeDamage(int dmg);
	virtual bool attack(Character* opponent) = 0; // returns true if the attack was successful
	//virtual void moveCharacter() = 0;

	int getSTR();
	int getDEX();
	int getLUK();
	int getDamage();
	int getHP();
	Mesh* getMesh();

	void setSTR(int newSTR);
	void setDEX(int newDEX);
	void setLUK(int newLUK);
	void setDamage(int newDamage);
	void setHP(int newHP);
	void setMesh(Mesh* newMesh);

	int i_movementCost;
	int i_stepsTaken;
	Mesh* characterMesh;
	bool b_tookAction; // If the unit has already moved/attacked this turn
protected:
	Weapon* weapon;
	Armor* armor;
	int i_HP;
	int i_Damage;
	// Stats
	int i_STR; // Determines the unit's attack power
	int i_DEX; // Determines the accuracy of the attacks of this unit
	int i_LUK; // Determines the avoidability of this unit
};

#endif CHARACTER_H
