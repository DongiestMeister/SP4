#include "AI_DefenceFSM.h"
#include "../AStar/AStar.h"
#include "SceneManager.h"
#include "../PlayerInfo.h"

AI_DefenceFSM::AI_DefenceFSM(Character *character, AI_DefenceFSM::Defence_Strategy init_state)
{
	this->character = character;
	target = nullptr;
	b_foundEnemyPath = false;
	b_foundReturnPath = false;
	state = init_state;
	b_isDone = false;
	originalState = init_state;
	originalPosition = character->getPos();// (character->getPos().x, character->getPos().y);
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
		if (target != nullptr)
		{
			if (character->getCurrentHP() <= 20)
			{
				state = RETREAT;
			}
			else
			{
				state = CHASE_DEF;
			}
		}
		//std::cout << "r11" << std::endl;
	}
	else if (is_stationary && target != nullptr)
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

		b_foundEnemyPath = SearchForPath(character->i_movementCost, target->getPos());	//search path creates unitPath[] elements
		//std::cout << "1" << std::endl;

		if (b_foundEnemyPath && unitPath.size() <= 3)
		{
			map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
			map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;

		}
		else
		{
			if (!(character->getPos() == originalPosition))
			{
				/*ReturnToPosition(dt);*/
				state = RETURN_TO_POSITION;
			}
			else
			{
				state = IDLE;
				b_isDone = true;
			}
		}
	}



	if (b_foundEnemyPath)
	{
		
		//if unit != took action, = current unit to move
		if (!character->b_tookAction)
		{
			

			//must be called AFTER setting unitPath
			MoveUnit(dt);

			if (unitPath.size() <= 0)
			{
				b_foundEnemyPath = false;
				if (b_reachEnd)
				{
					b_reachEnd = false;
					state = ATTACK;
					
				}
				else
				{
					b_isDone = true;
					state = IDLE;
				}
			}

		}
		else if (!(character->getPos() == originalPosition))
		{
			/*ReturnToPosition(dt);*/
			state = RETURN_TO_POSITION;
			b_foundEnemyPath = false;
			
			//std::cout << "r2" << std::endl;

		}
		else	//reached end of cost/limit allowed to move
		{
			b_foundEnemyPath = false;
			state = IDLE;
			
			b_isDone = true;
			
		}
	}
}
		
	


void AI_DefenceFSM::ReturnToPosition(double dt)
{
	
	if (!b_foundReturnPath)
	{
		//look for return path
		b_foundReturnPath = SearchForPath(character->i_movementCost, originalPosition);
		std::cout << "move cost : " << character->i_movementCost << std::endl;

		if (b_foundReturnPath)
		{
			//set start and end if found return path
			map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
			map->theScreenMap[unitPath[unitPath.size() -1 ].y][unitPath[unitPath.size() - 1].x] = 2;
			std::cout << "setting : " << unitPath[unitPath.size() - 1].x << " , " << unitPath[unitPath.size() - 1].y << std::endl;

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
		std::cout << "dest : " << unitPath[unitPath.size() - 1].x << " , " << unitPath[unitPath.size() - 1].y << std::endl;
		if (!character->b_tookAction)
		{
			MoveUnit(dt);

			if (unitPath.size() <= 0)
			{
				if (b_reachEnd)
				{
					std::cout << "search" << std::endl;
					b_reachEnd = false;
					state = IDLE;
				}
				else
				{
					std::cout << "stop" << std::endl;
					b_isDone = true;
				}
			}
			else
			{
				std::cout << "case" << std::endl;
				std::cout << "currPos : " << character->getPos().x << " , " << character->getPos().y << std::endl;
				std::cout << "target : " << target->getPos().x << " , " << target->getPos().y << std::endl;
				state = IDLE;
				b_isDone = true;
			}
			
		}
		
	}
}

void AI_DefenceFSM::Chase(double dt)
{
	//check for paths (target is set from searchnearest before this func)
	if (!b_foundEnemyPath)
	{
		b_foundEnemyPath = SearchForPath(character->i_movementCost, target->getPos());	//search path creates unitPath[] elements

		if (b_foundEnemyPath)
		{
			map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
			map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;

		}
	}


	//if path exists
	if (b_foundEnemyPath)
	{
		//if unit != took action, = current unit to move
		if (!character->b_tookAction)
		{
			//DEFENCE CHASE = if target is out of reach, return to IDLE state

			//must be called AFTER setting unitpath
			MoveUnit(dt);

			if (unitPath.size() <= 0)
			{
				b_foundEnemyPath = false;
				if (b_reachEnd)
				{
					b_reachEnd = false;
					state = ATTACK;
				}
				else
				{
					b_isDone = true;
					state = IDLE;
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

