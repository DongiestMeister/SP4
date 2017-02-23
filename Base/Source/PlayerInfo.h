#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "SingletonTemplate.h"
#include "Character/Character.h"
#include "Character/Item.h"
#include "Level.h"

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

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
	Level *level;

	bool b_attacking;
	bool addCharacterToParty(Vector2 pos, Character* newUnit, int key);
	void removeCharacterFromParty(int key);
	void addCharacter(Vector2 pos, Character* newUnit); // adds a character to the list of characters that you have
	void addCharacterToEnemies(Vector2 pos, Character* newUnit); //adds a character to the list of enemies
	void addItem(Item* itemToAdd);
	void loadWeaponsFromCSV(const string filepath);
	void loadArmorFromCSV(const string filepath);
	void saveInventoryToCSV(const string filepath);
protected:



private:
	


};



#endif