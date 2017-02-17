#ifndef AI_FSM_H
#define AI_FSM_H
#include "../TileMap.h"

class AI_FSM
{
public:
	AI_FSM();
	virtual ~AI_FSM();

	TileMap *map;
	Character *character;
	virtual bool Update(double dt) { return false; }

	bool SearchPath();
	bool SearchPathWithTerrain();

	/*Search_funts sets :
		- target
		- nearestdistance	*/
	/*void SearchNearest();
	void SearchLowestHP();*/
	
	void SearchNearestWithHP();


	Character *target;
	vector<Vector2> unitPath;

	float f_speed;

	// Stuff for in-loop movement
	bool b_isDone;
};



#endif


