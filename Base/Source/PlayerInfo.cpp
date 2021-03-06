#include "PlayerInfo.h"
#include "MeshBuilder.h"
#include "Lua\LuaInterface.h"

PlayerInfo::PlayerInfo()
{
	player = nullptr;
	enemy = nullptr; 
	b_attacking = true;
	level = nullptr;
	gold = 100000;
	b_bonus = false;
}

PlayerInfo::~PlayerInfo()
{
	party.clear();

	for (CharactersList::iterator it = availableUnits.begin(); it != availableUnits.end();)
	{
		if (*it)
		{
			delete (*it);
			it = availableUnits.erase(it);
		}
	}
	for (CharactersList::iterator it = enemies.begin(); it != enemies.end();)
	{
		if (*it)
		{
			delete (*it);
			it = enemies.erase(it);
		}
	}
	for (ItemList::iterator it = inventory.begin(); it != inventory.end();)
	{
		if (*it)
		{
			delete (*it);
			it = inventory.erase(it);
		}
	}

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

void PlayerInfo::addItemToInventory(Item* itemToAdd)
{
	inventory.push_back(itemToAdd);
}

void PlayerInfo::loadWeaponsFromCSV(const string filepath)
{
	std::ifstream file(filepath);
	if (file.is_open())
	{
		while (file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			size_t pos = aLineOfText.find("/");
			if (pos != string::npos)
			{
				aLineOfText = aLineOfText.substr(0, pos);
			}
			if (aLineOfText.empty())
				continue;
			Weapon* weapon = new Weapon();
			string token;
			std::istringstream iss(aLineOfText);
			int counter = 1;
			while (getline(iss, token, ','))
			{
				switch (counter)
				{
				case 1:
					weapon->s_Name = token;
					break;
				case 2:
					weapon->i_damageValue = stoi(token);
					break;
				case 3:
					weapon->i_weaponAccuracy = stoi(token);
					break;
				case 4:
					if (token == "default")
						weapon->s_ownerName = "";
					else
						weapon->s_ownerName = token;
					break;
				case 5:
					 if (token == "FALSE")
						weapon->b_isEquippedToSomeone = false;
					 else if (token == "TRUE")
						weapon->b_isEquippedToSomeone = true;
					break;
				case 6:
					weapon->i_Price = stoi(token);
					break;
				case 7:
					if (token == "default")
						weapon->itemPortrait = "swordPortrait";
					else
						weapon->itemPortrait = token;
					break;
				}
				counter += 1;
			}
			shop.push_back(weapon);
		}
	}
	file.close();
}

void PlayerInfo::loadArmorFromCSV(const string filepath)
{
	std::ifstream file(filepath);
	if (file.is_open())
	{
		while (file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			size_t pos = aLineOfText.find("/");
			if (pos != string::npos)
			{
				aLineOfText = aLineOfText.substr(0, pos);
			}
			if (aLineOfText.empty())
				continue;

			Armor* armor = new Armor();
			string token;
			std::istringstream iss(aLineOfText);
			int counter = 1;
			while (getline(iss, token, ','))
			{
				switch (counter)
				{
				case 1:
					armor->s_Name = token;
					break;
				case 2:
					armor->i_strBoost = stoi(token);
					break;
				case 3:
					armor->i_dexBoost = stoi(token);
					break;
				case 4:
					armor->i_lukBoost = stoi(token);
					break;
				case 5:
					armor->i_hpBoost = stoi(token);
					break;
				case 6:
					if (token == "default")
						armor->s_ownerName = "";
					else
						armor->s_ownerName = token;
					break;
				case 7:
					if (token == "FALSE")
						armor->b_isEquippedToSomeone = false;
					else if (token == "TRUE")
						armor->b_isEquippedToSomeone = true;
					break;
				case 8:
					armor->i_Price = stoi(token);
					break;
				case 9:
					if (token == "default")
						armor->itemPortrait = "armorPortrait";
					else
						armor->itemPortrait = token;
					break;
				}
				counter += 1;
			}
			shop.push_back(armor);
		}
	}
	file.close();
}

void PlayerInfo::loadCharactersFromCSV(const string filepath)
{
	std::ifstream file(filepath);
	if (file.is_open())
	{
		while (file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);


			size_t pos = aLineOfText.find("#");
			if (pos != string::npos)
			{
				aLineOfText = aLineOfText.substr(0, pos);
			}
			if (aLineOfText.empty())
				continue;

			Character* character;
			string token;
			std::istringstream iss(aLineOfText);
			int counter = 1;
			while (getline(iss, token, ','))
			{
				switch (counter)
				{
				case 1:
					if (token == "Melee")
						character = new MeleeCharacter("");
					else if (token == "Ranged")
						character = new RangedCharacter("");
					break;
				case 2:
					character->setName(token);
					break;
				case 3:
					character->setHP(std::stoi(token));
					break;
				case 4:
					character->setBaseSTR(std::stoi(token));
					break;
				case 5:
					character->setBaseDEX(std::stoi(token));
					break;
				case 6:
					character->setBaseLUK(std::stoi(token));
					break;
				case 7:
					character->i_movementCost = std::stoi(token);
					break;
				case 8:
					character->i_attackRange = std::stoi(token);
					break;
				case 9:
					character->setPortrait(token);
					break;
				case 10:
					character->s_animationName = token;
					break;
				case 11:
					character->i_frames = atoi(token.c_str());
					break;
				}
				counter += 1;
			}
			character->calculateStats();
			unitsDataBase.push_back(character);
			unitsNotOwned.push_back(character);
		}
	}
	file.close();
}

void PlayerInfo::LoadPlayerCharacters()
{
	string charnames = CLuaInterface::GetInstance()->getStringValue("characters", CLuaInterface::GetInstance()->theCharactersState);
	
	string token;
	std::istringstream iss(charnames);
	while (getline(iss, token, ','))
	{
		for (CharactersList::iterator it = unitsNotOwned.begin(); it != unitsNotOwned.end();)
		{
			Character *unit = *it;
			if (unit->getName() == token)
			{
				availableUnits.push_back(unit);
				it = unitsNotOwned.erase(it);
				break;
			}
			else
			{
				it++;
			}
		}
	}

	string items = CLuaInterface::GetInstance()->getStringValue("items", CLuaInterface::GetInstance()->theCharactersState);

	std::istringstream iss2(items);
	while (getline(iss2, token, ','))
	{
		for (ItemList::iterator it = shop.begin(); it != shop.end();)
		{
			Item *unit = *it;
			if (unit->s_Name == token)
			{
				inventory.push_back(unit);
				it = shop.erase(it);
				break;
			}
			else
			{
				it++;
			}
		}
	}
}

void PlayerInfo::SavePlayerCharacters()
{
	string save;
	for (int i = 0; i < availableUnits.size(); ++i)
	{
		save += availableUnits[i]->getName() + ",";
	}

	CLuaInterface::GetInstance()->SaveStringValue("characters", save, true, "Image//Lua//Characters.lua");

	string save2;
	for (int i = 0; i < inventory.size(); ++i)
	{
		save2 += inventory[i]->s_Name + ",";
	}

	CLuaInterface::GetInstance()->SaveStringValue("items", save2, false, "Image//Lua//Characters.lua");
}