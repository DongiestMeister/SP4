#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "SingletonTemplate.h"
#include "Character/Character.h"
#include "Character/Item.h"

#include <string>
#include <vector>
#include <map>

using std::string;
typedef std::vector<Item*> ItemList;

class PlayerInfo : public Singleton<PlayerInfo>
{
	friend Singleton<PlayerInfo>;

public:

	CharactersList party; // Party to be used on the map, should only be capped at 4 units
	CharactersList availableUnits; // All of the available units

	CharactersList enemies; // All of the available enemies
	PlayerInfo();
	~PlayerInfo();


	Character *player;
	Character *enemy;
	ItemList inventory; // Vector of items that the player has. Includes Weapons, Armors, Recovery Items etc...

	bool b_attacking;
	bool addCharacterToParty(Vector2 pos, Character* newUnit, int key);
	void removeCharacterFromParty(int key);
	void addCharacter(Vector2 pos, Character* newUnit); // adds a character to the list of characters that you have
	void addCharacterToEnemies(Vector2 pos, Character* newUnit); //adds a character to the list of enemies

protected:



private:
	


};



#endif