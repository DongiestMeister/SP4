#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "SingletonTemplate.h"
#include "Character\Character.h"

#include <string>
#include <vector>
#include <map>

using std::string;

class PlayerInfo : public Singleton<PlayerInfo>
{
	friend Singleton<PlayerInfo>;

public:

	CharactersList party; // Party to be used on the map, should only be capped at 4 units
	CharactersList availableUnits; // All of the available units
	CharactersList enemies; // All of the available units
	PlayerInfo() { player = NULL; enemy = NULL;}
	~PlayerInfo() {}

	Character *player;
	Character *enemy;

	bool b_attacking = true;

	bool addCharacterToParty(Vector2 pos, Character* newUnit, int key)
	{
		if (party.size() >= 4)
			return false;
		for (auto pt : party)
		{
			if (pt->i_idInParty == key)
				return false; // Shows that this key already exists
		}
		newUnit->setPos(pos);
		newUnit->b_inParty = true;
		newUnit->i_idInParty = key;
		party.push_back(newUnit);
		return true;
	}

	void removeCharacterFromParty(int key)
	{
		int counter = 0;
		for (CharactersList::iterator pt = party.begin(); pt != party.end(); pt++, counter++)
		{
			if ((*pt)->i_idInParty == key)
			{
				for (CharactersList::iterator pt = availableUnits.begin(); pt != availableUnits.end(); pt++)
				{
					if ((*pt)->i_idInParty == key)
					{
						(*pt)->i_idInParty = -1;
						(*pt)->b_inParty = false;
					}
				}
				party.erase(party.begin() + counter);
				for (CharactersList::iterator pt2 = party.begin(); pt2 != party.end(); pt2++)
				{
					(*pt2)->i_idInParty = std::distance(party.begin(), pt2);
				}
				break;
			}
		}
		
	}
	void addCharacter(Vector2 pos, Character* newUnit) // adds a character to the list of characters that you have
	{
		newUnit->setPos(pos);
		availableUnits.push_back(newUnit);
	}

	void addCharacterToEnemies(Vector2 pos, Character* newUnit)
	{
		newUnit->setPos(pos);
		enemies.push_back(newUnit);
	}
protected:



private:
	


};



#endif