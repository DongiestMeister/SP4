#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{
	player = nullptr;
	enemy = nullptr; 
	b_attacking = true;
}

PlayerInfo::~PlayerInfo()
{
	for (CharactersList::iterator it = party.begin(); it != party.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	for (CharactersList::iterator it = availableUnits.begin(); it != availableUnits.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	for (CharactersList::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	for (ItemList::iterator it = inventory.begin(); it != inventory.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
	//if (player)
	//{
	//	delete player;
	//	player = nullptr;
	//}
	//if (enemy)
	//{
	//	delete enemy;
	//	enemy = nullptr;
	//}
}

bool PlayerInfo::addCharacterToParty(Vector2 pos, Character* newUnit, int key)
{
	if (party.size() >= 4)
		return false;
	for (CharactersList::iterator pt = party.begin(); pt != party.end(); pt++)
	{
		if ((*pt)->i_idInParty == key)
			return false; // Shows that this key already exists
	}
	newUnit->setPos(pos);
	newUnit->b_inParty = true;
	newUnit->i_idInParty = key;
	party.push_back(newUnit);
	return true;
}

void PlayerInfo::removeCharacterFromParty(int key)
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

void PlayerInfo::addCharacter(Vector2 pos, Character* newUnit)
{
	newUnit->setPos(pos);
	availableUnits.push_back(newUnit);
}

void PlayerInfo::addCharacterToEnemies(Vector2 pos, Character* newUnit)
{
	newUnit->setPos(pos);
	enemies.push_back(newUnit);
}