#include "AI_OffenceFSM.h"
#include "../AStar/AStar.h"
#include "SceneManager.h"
#include "../PlayerInfo.h"

AI_OffenceFSM::AI_OffenceFSM(Character *character)
{
	this->character = character;
	target = NULL;
	b_isDone = false;
	b_foundEnemyPath = false;
	state = CHASE_ALL;
	//Ai_parent = new AI_FSM();
}


AI_OffenceFSM::~AI_OffenceFSM()
{

}

bool AI_OffenceFSM::Update(double dt)
{
	if (!character->b_tookAction)
	{
		switch (state)
		{
		case ATTACK:
			Attack(); 
			break;
		case CHASE_ALL:
			Chase(dt);
			break;
		case CHASE_RANGED:
			break;
		case CHASE_MELEE:
			break;
		case CHASE_CAPTURETARGET:
			break;
		default:
			break;
		}
	}

	if (target == NULL || b_isDone)
	{
		b_isDone = false;
		return true;
	}
	return false;
}


void AI_OffenceFSM::Chase(double dt)
{
	//Set target to current unit that is nearest
	SearchNearestWithHP();

	if (!b_foundEnemyPath)
	{
		b_foundEnemyPath = SearchForPath(character->i_movementCost, target->getPos());

		if (b_foundEnemyPath) //&& unitPath.size() > 0)
		{
			map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
			map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;
		}
		else
		{
			b_isDone = true;
			b_foundEnemyPath = false;
		}
	}
	

	//if unit != took action, = current unit to move
	if (!character->b_tookAction)
	{
		//ATTACK CHASE = if target is out of reach, Keep chasing
		// if dist btwn 1st pos & unit pos != 0, 
		MoveUnit(dt);

		if ((unitPath.size() <= 0))
		{
			state = ATTACK;
		}
	}
}
	
