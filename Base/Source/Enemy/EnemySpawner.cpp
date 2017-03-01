#include "EnemySpawner.h"
#include "../Character/MeleeCharacter.h"
#include "../Character/RangedCharacter.h"
#include <sstream>
#include <fstream>
#include "../TileMap.h"

using std::string;
using std::ifstream;
using std::istringstream;

EnemySpawner::EnemySpawner()
{
	map = nullptr;
}

EnemySpawner::~EnemySpawner()
{

}

void EnemySpawner::Init(TileMap *map)
{
	this->map = map;
}

void EnemySpawner::SpawnWave(int i_turn)
{
	int spawnpointIterator = 0;
	for (vector<Enemy>::iterator it = spawnEnemyList.begin(); it != spawnEnemyList.end();)
	{
		Enemy *enemy = &*it;
		if (enemy->i_Spawnturn == i_turn)
		{
			for (int i = 0; i < map->enemySpawnPoints.size(); ++i)
			{
				Vector2 spawnPoint = map->enemySpawnPoints[i];
				if (map->theScreenMap[spawnPoint.y][spawnPoint.x] != 2)
				{
					Character *knight;
					if (enemy->c_class == Enemy::MELEE)
					{
						knight = new MeleeCharacter(enemy->s_name);
					}
					else if (enemy->c_class == Enemy::RANGE)
					{
						knight = new RangedCharacter(enemy->s_name);
					}
					if (enemy->type == Enemy::DEFENCE)
					{
						knight->strategy = Character::DEFENCE;
					}
					else if (enemy->type == Enemy::NEUTRAL)
					{
						knight->strategy = Character::NEUTRAL;
					}
					else if (enemy->type == Enemy::OFFENCE)
					{
						knight->strategy = Character::OFFENCE;
					}
					knight->setPortrait(enemy->s_charPortrait);
					knight->setAnimation(enemy->s_animation, enemy->i_frames);
					knight->setHP(enemy->i_maxHP);
					knight->setBaseSTR(enemy->i_baseStr);
					knight->setBaseDEX(enemy->i_baseDex);
					knight->setBaseLUK(enemy->i_baseLuk);
					knight->i_movementCost = enemy->i_movement;
					knight->calculateStats();
					map->AddEnemy(spawnPoint.x, spawnPoint.y, knight);
					it = spawnEnemyList.erase(it);
					break;
				}
			}
			if (spawnpointIterator == map->enemySpawnPoints.size())
			{
				break;
			}
		}
		else
		{
			++it;
		}
	}
}

void EnemySpawner::AddToSpawnList(Enemy enemy)
{
	spawnEnemyList.push_back(enemy);
}

void EnemySpawner::ClearSpawns()
{
	spawnEnemyList.clear();
}

bool EnemySpawner::LoadSpawns(const string filename)
{
	spawnEnemyList.clear();

	ifstream file(filename.c_str());
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

			string token;
			istringstream iss(aLineOfText);
			int i_typeCounter = 0;
			Enemy enemy;

			while (getline(iss, token, ','))
			{
				switch (i_typeCounter)
				{
				case 0:
					enemy = enemyMap[atoi(token.c_str())];
					break;
				case 1:
					enemy.i_Spawnturn = atoi(token.c_str());
					break;
				}

				i_typeCounter++;
			}

			spawnEnemyList.push_back(enemy);
		}
	}

	file.close();
	return true;
}

bool EnemySpawner::LoadEnemies(const string filename)
{
	enemyMap.clear();

	ifstream file(filename.c_str());
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

			string token;
			istringstream iss(aLineOfText);
			int i_typeCounter = 0;

			Enemy enemy;

			while (getline(iss, token, ','))
			{
				switch (i_typeCounter)
				{
				case 0:
					enemy.s_name = token;
					break;
				case 1:
					if (token == "DEFENCE")
					{
						enemy.type = Enemy::DEFENCE;
					}
					else if (token == "IDLE")
					{
						enemy.type = Enemy::NEUTRAL;
					}
					else if (token == "OFFENCE")
					{
						enemy.type = Enemy::OFFENCE;
					}
					break;
				case 2:
					if (token == "MELEE")
					{
						enemy.c_class = Enemy::MELEE;
					}
					else if (token == "RANGE")
					{
						enemy.c_class = Enemy::RANGE;
					}
					break;
				case 3:
					enemy.i_maxHP = atoi(token.c_str());
					break;
				case 4:
					enemy.i_baseStr = atoi(token.c_str());
					break;
				case 5:
					enemy.i_baseDex = atoi(token.c_str());
					break;
				case 6:
					enemy.i_baseLuk = atoi(token.c_str());
					break;
				case 7:
					enemy.i_movement = atoi(token.c_str());
					break;
				case 8:
					enemy.s_charPortrait = token;
					break;
				case 9:
					enemy.s_animation = token;
					break;
				case 10:
					enemy.i_frames = atoi(token.c_str());
					break;
				case 11:
					enemyMap[atoi(token.c_str())] = enemy;
					break;
					
				}
				i_typeCounter++;
			}
		}
	}

	file.close();
	return true;
}