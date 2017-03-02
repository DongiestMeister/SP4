#include "AI_DefenceFSM.h"
#include "../AStar/AStar.h"
#include "SceneManager.h"
#include "../PlayerInfo.h"

AI_DefenceFSM::AI_DefenceFSM(Character *character, AI_DefenceFSM::Defence_Strategy init_state)
{
	this->character = character;
	target = nullptr;
	b_foundReturnPath = false;
	state = init_state;
	b_isDone = false;
	originalState = init_state;
	//originalPosition = character->getPos();// (character->getPos().x, character->getPos().y);
}

AI_DefenceFSM::~AI_DefenceFSM()
{

}

bool AI_DefenceFSM::Update(double dt)
{
	if (!character->b_tookAction)
	{
		switch (state)
		{
		case IDLE:
			if (originalState == IDLE)
				Idle(dt, false);
			else
				Idle(dt, true);
			
			break;
		case CHASE_DEF:
			Chase(dt);
			break;
		case ATTACK:
			Attack();
			if (originalState == IDLE)
				state = IDLE;
			else
				state = STATIONARY;
			break;
		case STATIONARY:
			Idle(dt, true);//Stationary(dt);
			break;
		case RETURN_TO_POSITION:
			ReturnToPosition(dt);
			break;
		case RETREAT:
			Retreat(dt);
			break;
		case DEFEND_CAPTURE:
			break;
		default:
			break;
		}
	}

	if (target == nullptr || b_isDone)
	{
		b_isDone = false;
		return true;
	}
	return false;
}

void AI_DefenceFSM::Idle(double dt, bool is_stationary)
{

	//Set target of current unit to nearest target
	SearchNearestWithHP();

	if (!is_stationary)
	{
		if (target != nullptr && !b_foundEnemyPath)
		{
			{
				state = CHASE_DEF;
			}
		}
		//std::cout << "r11" << std::endl;
	}
	else if (is_stationary && target != nullptr && !b_foundEnemyPath)
	{
		//state = STATIONARY;
		Stationary(dt);
		
	}
	else
	{
		b_isDone = true;

	}
}

void AI_DefenceFSM::Stationary(double dt)
{

	if (!b_foundEnemyPath)
	{

		b_foundEnemyPath = true;
		//total unit path is less than cost left
		if (unitPath.size() < character->i_attackRange + 1 && b_reachEnd)
		{
			//b_reachEnd = true;
		}
		//if melee and unitPath is less than 3
		else
		{
			b_reachEnd = false;
			b_foundEnemyPath = false;
			b_isDone = true;
			state = IDLE;
			target = NULL;
		}
	}





	if (target != NULL)
	{
		if (!character->b_tookAction)
		{
			if (b_reachEnd)
			{
				b_reachEnd = false;
				state = ATTACK;
			}
		}
	}

}
		
void AI_DefenceFSM::ReturnToPosition(double dt)
{
	target->setPos(originalPosition);
	
	if (!b_foundReturnPath)
	{
		//look for return path
		//b_foundReturnPath = SearchForPath(character->i_movementCost, originalPosition);
		//std::cout << "move cost : " << character->i_movementCost << std::endl;

		if (b_foundReturnPath)
		{
			//set start and end if found return path
			map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
			map->theScreenMap[unitPath[unitPath.size() -1 ].y][unitPath[unitPath.size() - 1].x] = 2;
			//std::cout << "setting : " << unitPath[unitPath.size() - 1].x << " , " << unitPath[unitPath.size() - 1].y << std::endl;

			/*Return path found path, but short of one unit away (detect player offset)*/
		}
		else
		{
			//there is no return path, clear bool and return turn(done)
			b_foundReturnPath = false;
			b_isDone = true;
		}
	}

	
	if (b_foundReturnPath)
	{
		//std::cout << "dest : " << unitPath[unitPath.size() - 1].x << " , " << unitPath[unitPath.size() - 1].y << std::endl;
		if (!character->b_tookAction)
		{
			MoveUnit(dt);

			if (unitPath.size() <= 0)
			{
				if (b_reachEnd)
				{
					//std::cout << "search" << std::endl;
					b_reachEnd = false;
					state = IDLE;
				}
				else
				{
					//std::cout << "stop" << std::endl;
					b_isDone = true;
				}
			}
			else
			{
				//std::cout << "case" << std::endl;
				//std::cout << "currPos : " << character->getPos().x << " , " << character->getPos().y << std::endl;
				//std::cout << "target : " << target->getPos().x << " , " << target->getPos().y << std::endl;
				state = IDLE;
				b_isDone = true;
			}
			
		}
		
	}
}

void AI_DefenceFSM::Chase(double dt)
{
	if (!b_foundEnemyPath)
	{

		b_foundEnemyPath = true;
		//total unit path is less than cost left
		if (unitPath.size() < character->i_movementCost)
		{
			b_reachEnd = true;
		}
		//if melee and unitPath is less than 3
		if (character->i_attackRange <= 1 && b_reachEnd)
		{
			map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
			map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;
		}
		//limit movement for defence unit
		else if (b_reachEnd)
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
		//if unitPath over size limit, return to idle
		else
		{
			b_foundEnemyPath = false;
			b_isDone = true;
			target = NULL;
			state = IDLE;
		}
	}
	
	if (target != NULL)
	{
		if (!character->b_tookAction)
		{

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
			}
		}
	}


}
		
		



void AI_DefenceFSM::Retreat(double dt)
{
	float nearestdist = FLT_MAX;

	//Not using SearchNearest() because it takes "map->characters" which only detects "players"
	for (CharactersList::iterator it = map->enemies.begin(); it != map->enemies.end(); ++it)
	{
		Character* ally = *it;

		//if curr enemy to their ally < nearestdist, update nearestdist
		if (((character->getPos() - ally->getPos()).Length()) < nearestdist)
		{
			nearestdist = ((character->getPos() - ally->getPos()).Length());
			target = ally;
		}


		if (!(unitPath[0] - character->getPos()).IsZero())
		{
			Vector2 tempStep = (unitPath[0] - character->getPos()).Normalized();
			character->setPos(Vector2(character->getPos() + (tempStep*dt*f_speed)));
		}
		if ((unitPath[0] - character->getPos()).Length() < 0.1f)
		{
			character->setPos(unitPath[0]);
			unitPath.erase(unitPath.begin());
		}
	}
}

void AI_DefenceFSM::SetAIState(AI_DefenceFSM::Defence_Strategy _state)
{
	state = _state;
}

