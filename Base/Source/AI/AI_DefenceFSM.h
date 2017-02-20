#ifndef AI_DEFENCE_H
#define AI_DEFENCE_H

#include "AI_FSM.h"

class AI_DefenceFSM : public AI_FSM
{
public:
	AI_DefenceFSM(Character *character);
	~AI_DefenceFSM();
	enum Defence_Strategy
	{
		//HEAL_MEMBER,
		//DEFEND_POINT,
		RETREAT,
		IDLE,
		CHASE,
		ATTACK,

	};

	Defence_Strategy state;

	void Idle();	//Detect closest player
	void Chase(double dt);
	void Attack();
	void Retreat();	//Retreats to closest ally

	bool Update(double dt);


	// Stuff added for in-loop movement
	bool b_foundEnemyPath;
};





#endif