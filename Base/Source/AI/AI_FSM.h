#ifndef AI_FSM_H
#define AI_FSM_H
#include "../TileMap.h"

class AI_FSM
{
public:
	AI_FSM() { character = nullptr; }
	virtual ~AI_FSM() { if (character) delete character; character = nullptr; }

	TileMap *map;
	Character *character;
	virtual void Update(double dt) { ; }
};



#endif


