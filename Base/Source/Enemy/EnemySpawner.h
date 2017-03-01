#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H

#include <vector>
#include <string>
#include "Vector3.h"
#include <map>

using std::vector;
using std::string;
using std::map;

class TileMap;

struct Enemy
{
	enum TYPE
	{
		DEFENCE,
		OFFENCE,
		NEUTRAL,
		TYPE_TOTAL
	};

	enum CLASS
	{
		MELEE,
		RANGE,
		CLASS_TOTAL
	};


	Enemy(TYPE type, int i_Spawnturn) {
		this->type = type; this->i_Spawnturn = i_Spawnturn; s_name = "Knight"; c_class = MELEE; i_maxHP = 0;
		i_baseStr = 0;
		i_baseDex = 0;
		i_baseLuk = 0;
		i_movement = 0;
		i_frames = 0;
	}
	Enemy() {
		this->type = DEFENCE; this->i_Spawnturn = 0; s_name = "Knight"; c_class = MELEE; i_maxHP = 0;
		i_baseStr = 0;
		i_baseDex = 0;
		i_baseLuk = 0;
		i_movement = 0;
		i_frames = 0;
	}
	~Enemy() {}

	TYPE type;
	string s_name;
	int i_Spawnturn;
	CLASS c_class;
	int i_maxHP;
	int i_baseStr;
	int i_baseDex;
	int i_baseLuk;
	int i_movement;
	string s_charPortrait;
	string s_animation;
	int i_frames;
};

class EnemySpawner
{
public:
	EnemySpawner();
	~EnemySpawner();

	void Init(TileMap *map);
	void SpawnWave(int i_turn);
	void AddToSpawnList(Enemy enemy);
	void ClearSpawns();
	bool LoadSpawns(const string filename);
	bool LoadEnemies(const string filename);
	map<int, Enemy> enemyMap;
private:
	TileMap *map;
	vector<Enemy> spawnEnemyList;
};


#endif