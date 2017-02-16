#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "SingletonTemplate.h"
#include "Character\Character.h"

#include <string>
#include <vector>

using std::string;

class PlayerInfo : public Singleton<PlayerInfo>
{
	friend Singleton<PlayerInfo>;

public:
	PlayerInfo() { player = NULL; enemy = NULL; }
	~PlayerInfo() {}

	Character *player;
	Character *enemy;

	bool b_attacking = true;
	CharactersList party; // Party to be used on the map, should only be capped at 4 units
	CharactersList availableUnits; // All of the available units

	void addCharacterToParty(Vector2 pos, Character* newUnit)
	{
		newUnit->setPos(pos);
		party.push_back(newUnit);
	}

	void addCharacter(Vector2 pos, Character* newUnit) // adds a character to the list of characters that you have
	{
		newUnit->setPos(pos);
		availableUnits.push_back(newUnit);
	}
protected:



private:
	


};



#endif