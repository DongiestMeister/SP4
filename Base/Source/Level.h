#ifndef LEVEL_H
#define LEVEL_H

#include "Vector2.h"

enum VICTORY_CONDITION
{
	KILL,
	SURVIVE,
	CAPTURE
};

class Level
{
public:
	Level() { b_completed = b_win = false; i_mapSizeX = i_mapSizeY = 0; i_surviveTurns = 10; }
	Level(VICTORY_CONDITION newcond, Vector2 newpos) { condition = newcond; pos = newpos; b_completed = b_win = false; i_mapSizeX = i_mapSizeY = 0; i_surviveTurns = 10; }
	~Level() {}
	VICTORY_CONDITION condition;
	Vector2 pos;
	bool b_completed;
	bool b_win;
	string s_enemyType;
	string s_enemySpawn;
	string s_mapName;
	int i_mapSizeX;
	int i_mapSizeY;
	int i_surviveTurns;
};

#endif