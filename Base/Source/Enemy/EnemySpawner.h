#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H

#include <vector>
#include <string>
#include "Vector3.h"
#include "../TileMap.h"

using std::vector;
using std::string;

struct Enemy
{
	enum TYPE
	{
		DEFENCE,
		TYPE_TOTAL
	};

	Enemy(TYPE type, int i_Spawnturn) { this->type = type; this->i_Spawnturn = i_Spawnturn; }
	~Enemy() {}

	TYPE type;
	int i_Spawnturn;
};

class EnemySpawner
{
public:
	EnemySpawner();
	~EnemySpawner();

	void Init(TileMap *map);
	void SpawnWave(int i_turn);
	void AddToSpawnList(Enemy enemy);
private:
	TileMap *map;
	vector<Enemy> spawnEnemyList;
};


#endif