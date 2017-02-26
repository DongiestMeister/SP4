#include "EnemySpawner.h"
#include "../Character/MeleeCharacter.h"

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
					Character *knight = new MeleeCharacter("Knight");
					knight->setPortrait("Knight");
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