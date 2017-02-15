#ifndef AI_DEFENCE_H
#define AI_DEFENCE_H

#include "AI_FSM.h"

class AI_DefenceFSM : public AI_FSM
{
public:
	AI_DefenceFSM();
	~AI_DefenceFSM();
	enum Defence_Strategy
	{
		//HEAL_MEMBER,
		//DEFEND_POINT,
		//RETREAT
		IDLE,
		CHASE,
		ATTACK
	};

	Defence_Strategy state;

	Unit *target;

	void Idle();
	void Chase();
	void Attack();

	vector<Vector2> unitPath;

	void Update(double dt);

	bool SearchPath();
};





#endif