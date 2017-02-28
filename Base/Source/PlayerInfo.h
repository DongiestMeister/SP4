#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "SingletonTemplate.h"
#include "Character/Character.h"
#include "Character/Item.h"
#include "Character/MeleeCharacter.h"
#include "Character/RangedCharacter.h"
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
	CharactersList availableUnits; // All of the units that the player has
	CharactersList unitsNotOwned;  // Units not owned by the player
	CharactersList unitsDataBase; // All the units loaded from csv

	CharactersList enemies; // All of the available enemies
	PlayerInfo();
	~PlayerInfo();


	Character *player;
	Character *enemy;
	ItemList inventory; // Vector of items that the player has. Includes Weapons, Armors, Recovery Items etc...
	ItemList shop;
	Level *level;
	bool b_bonus;

	int gold; // amount the gold the player has
	bool b_attacking;
	bool addCharacterToParty(Vector2 pos, Character* newUnit, int key);
	void removeCharacterFromParty(int key);
	// adds a character to the list of characters that you have
	void addCharacter(Vector2 pos, Character* newUnit);
	//adds a character to the list of enemies
	void addCharacterToEnemies(Vector2 pos, Character* newUnit); 
	void addItemToInventory(Item* itemToAdd);
	// items to be sold in the shop
	void loadWeaponsFromCSV(const string filepath);
	// items to be sold in the shop
	void loadArmorFromCSV(const string filepath);
	void loadCharactersFromCSV(const string filepath);
	void saveInventoryToCSV(const string filepath);
protected:



private:
	


};



#endif