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
	Level() { b_completed = b_win = false; }
	Level(VICTORY_CONDITION newcond, Vector2 newpos) { condition = newcond; pos = newpos; b_completed = b_win = false; }
	~Level() {}
	VICTORY_CONDITION condition;
	Vector2 pos;
	bool b_completed;
	bool b_win;
};

#endif