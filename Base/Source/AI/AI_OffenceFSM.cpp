#include "AI_OffenceFSM.h"
#include "../AStar/AStar.h"
#include "SceneManager.h"
#include "../PlayerInfo.h"

AI_OffenceFSM::AI_OffenceFSM(Character *character)
{
	this->character = character;
	target = NULL;
	b_isDone = false;
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
			state = CHASE_ALL;
			break;
		case CHASE_ALL:
			Chase(dt);
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
	//No target & no Path
	if (target == NULL && !b_foundEnemyPath)
	{
		SearchNearestWithHP();	//Assigns Target , unitPath
		if (target != NULL)
		{
			b_foundEnemyPath = true;
			if (unitPath.size() < character->i_movementCost)
			{
				b_reachEnd = true;
			}
			//melee
			if (character->i_attackRange <= 1)
			{
				map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
				map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;
			}
			//ranged
			else
			{
				if (b_reachEnd)
				{
					map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
					if (unitPath.size() >= 2)
					{
						map->theScreenMap[unitPath[unitPath.size() - 2].y][unitPath[unitPath.size() - 2].x] = 2;
					}
					else
					{
						map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;
					}	
				}
				else
				{
					map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
					map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;
				}
			}
		}
		else
		{
			b_isDone = true;
		}
	}


	if (target != NULL)
	{
		if (!character->b_tookAction)
		{
			//ATTACK CHASE = if target is out of reach, Keep chasing
			// if dist btwn 1st pos & unit pos != 0, 
			MoveUnit(dt);

			if (character->i_attackRange == 1)
			{
				if (unitPath.size() <= 0)
				{
					if (b_reachEnd)
					{
						b_reachEnd = false;
						state = ATTACK;
					}
					else
					{
						b_foundEnemyPath = false;
						b_isDone = true;
						target = NULL;
					}
				}
			}
			else if (character->i_attackRange >= 2)
			{
				if (b_reachEnd)
				{
					if (unitPath.size() <= 1)
					{
						b_reachEnd = false;
						state = ATTACK;
					}
				}
				else
				{
					if (unitPath.size() <= 0)
					{
						b_foundEnemyPath = false;
						b_isDone = true;
						target = NULL;
					}
				}
			}
		}
	}
}
	
