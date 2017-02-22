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
		
		IDLE,
		CHASE_DEF,
		ATTACK,
		STATIONARY,
		RETURN_TO_POSITION,
		RETREAT,
		DEFEND_CAPTURE,
	};

	Defence_Strategy state;

	void Idle(bool is_stationary);	//Detect closest player
	void Chase(double dt);
	void Retreat(double dt);	//Retreats to closest ally
	void Stationary(double dt);			//only move when enemy close, return to position if player out of range
	void ReturnToPosition(double dt);

	void SetAIState(AI_DefenceFSM::Defence_Strategy _state);

	bool Update(double dt);


	// Stuff added for in-loop movement
	bool b_foundEnemyPath;

private:
	//Original Position set for Stationary units
	Vector2 originalPosition;
};





#endif