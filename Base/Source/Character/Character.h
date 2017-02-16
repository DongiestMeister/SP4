#ifndef CHARACTER_H
#define CHARACTER_H

#include "Weapon.h"
#include "Armor.h"
#include "MyMath.h"
#include "Mesh.h"
#include "Vector2.h"
#include <string>
#include <vector>

using std::string;

class Character
{
public:
	Character(); // Hp always default to 100
	virtual ~Character(){};
	void takeDamage(int dmg);
	virtual bool attack(Character* opponent) = 0; // returns true if the attack was successful
	//virtual void moveCharacter() = 0;
	virtual void equipWeapon(Weapon* newWeapon);
	virtual void equipArmor(Armor* newArmor);
	virtual void calculateStats();

	int getSTR();
	int getDEX();
	int getLUK();
	int getDamage();
	int getHP();
	Mesh* getMesh();
	Vector2 getPos();

	void setSTR(int newSTR);
	void setDEX(int newDEX);
	void setLUK(int newLUK);
	void setDamage(int newDamage);
	void setHP(int newHP);
	void setMesh(Mesh* newMesh);
	void setPos(Vector2 newPos);

	int i_movementCost;
	int i_stepsTaken;
	Mesh* characterMesh;
	bool b_tookAction; // If the unit has already moved/attacked this turn
	bool b_inParty; // If this unit is in the party
	int i_attackRange; // Range of attack of this character
protected:
	Weapon* weapon;
	Armor* armor;
	int i_HP;
	int i_Damage;
	Vector2 pos;
	// Stats
	int i_STR; // Determines the unit's attack power
	int i_DEX; // Determines the accuracy of the attacks of this unit
	int i_LUK; // Determines the avoidability of this unit
	string s_Name;
};

typedef std::vector<Character*> CharactersList;

#endif CHARACTER_H
