#ifndef AI_FSM_H
#define AI_FSM_H
#include "../TileMap.h"

class AI_FSM
{
public:
	AI_FSM();
	virtual ~AI_FSM() {  }

	TileMap *map;
	Character *character;
	virtual void Update(double dt) { ; }

	bool SearchPath();
	bool SearchPathWithTerrain();

	/*Search_funts sets :
		- target
		- nearestdistance	*/
	/*void SearchNearest();
	void SearchLowestHP();*/
	
	void SearchNearestWithHP();


	Unit *target;
	vector<Vector2> unitPath;

	float f_speed;
};



#endif


