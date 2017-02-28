#include "PlayerInfo.h"
#include "MeshBuilder.h"

PlayerInfo::PlayerInfo()
{
	player = nullptr;
	enemy = nullptr; 
	b_attacking = true;
	level = nullptr;
	gold = 500;
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
			while (getline(iss, token, ','))
			{
				if (weapon->s_Name == "")
					weapon->s_Name = token;
				else if (weapon->i_damageValue == -1)
					weapon->i_damageValue = stoi(token);
				else if (weapon->i_weaponAccuracy == -1)
					weapon->i_weaponAccuracy = stoi(token);
				else if (weapon->s_ownerName == "")
					weapon->s_ownerName = token;
				else if (token == "FALSE")
					weapon->b_isEquippedToSomeone = false;
				else if (token == "TRUE")
					weapon->b_isEquippedToSomeone = true;
				else if (weapon->i_Price == -1)
					weapon->i_Price = stoi(token);
			}
			weapon->itemPortrait = "swordPortrait";
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
			while (getline(iss, token, ','))
			{
				if (armor->s_Name == "")
					armor->s_Name = token;
				else if (armor->i_strBoost == -1)
					armor->i_strBoost = stoi(token);
				else if (armor->i_dexBoost == -1)
					armor->i_dexBoost = stoi(token);
				else if (armor->i_lukBoost == -1)
					armor->i_lukBoost = stoi(token);
				else if (armor->i_hpBoost == -1)
					armor->i_hpBoost = stoi(token);
				else if (armor->s_ownerName == "")
					armor->s_ownerName = token;
				else if (token == "FALSE")
					armor->b_isEquippedToSomeone = false;
				else if (token == "TRUE")
					armor->b_isEquippedToSomeone = true;
				else if (armor->i_Price == -1)
					armor->i_Price = stoi(token);
			}
			armor->itemPortrait = "armorPortrait";
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


			size_t pos = aLineOfText.find("/");
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
					character->set2DMesh(token);
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