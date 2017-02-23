#include "AI_DefenceFSM.h"
#include "../AStar/AStar.h"
#include "SceneManager.h"
#include "../PlayerInfo.h"

AI_DefenceFSM::AI_DefenceFSM(Character *character, AI_DefenceFSM::Defence_Strategy init_state)
{
	this->character = character;
	target = nullptr;
	b_foundEnemyPath = false;
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
		//std::cout << "r9" << std::endl;
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
	else if (is_stationary)
	{
		//state = STATIONARY;
		Stationary(dt);
	//	std::cout << "WAGA IDLE NI NARU" << std::endl;
		
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

		b_foundEnemyPath = SearchForPath();	//search path creates unitPath[] elements
		//std::cout << "1" << std::endl;

		if (b_foundEnemyPath)
		{
			map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
			map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;	
			
		}
		else
		{
			//std::cout << "ROUTE CANNOT BE FOUND" << std::endl;
			b_isDone = true;
		}
	}


	if (b_foundEnemyPath)
	{
		//std::cout << "3" << std::endl;
		//if unit != took action, = current unit to move
		if (!character->b_tookAction)
		{
			//STATIONARY CHASE = hardcoded fixed values so units dont move out of defending range
			if ((character->getPos() - target->getPos()).Length() <= 3)
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
						//std::cout << "r3" << std::endl;
					}
					else
					{
						b_isDone = true;
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
				std::cout << target->getPos().x << " : " << target->getPos().y << std::endl;
			}
		}
	}
}

void AI_DefenceFSM::ReturnToPosition(double dt)
{
	unitPath.clear();

	//Search and create path
	AStar search((int)character->getPos().x, (int)character->getPos().y, (int)originalPosition.x, (int)originalPosition.y, map);
	if (search.Search())
	{
		if (character->i_movementCost >= search.bestPath.size() - 1)
		{
			b_reachEnd = true;
		}
		for (int i = 0; i < character->i_movementCost && i < search.bestPath.size() - 1; ++i)
		{
			unitPath.push_back(search.bestPath[i]);
		}
	}

	if (!character->b_tookAction)
	{
		//RETURN TO POSITION : originalPosition is replaced from "target", to go back to "position"
		if ((character->getPos() - originalPosition).Length() <= character->i_movementCost)
		{

			MoveUnit(dt);

			if (unitPath.size() <= 0)
			{
				if (b_reachEnd)
				{
					b_reachEnd = false;
					state = IDLE;
					//std::cout << "rr4" << std::endl;
				}
				else
				{
					b_isDone = true;
				}
				//std::cout << "r5" << std::endl;
			}
			//std::cout << "r6" << std::endl;
		}
		else
		{
			state = IDLE;
			b_isDone = true;
		}
		//std::cout << "r7" << std::endl;
	}

	//b_isDone = true;
}

void AI_DefenceFSM::Chase(double dt)
{
	//check for paths (target is set from searchnearest before this func)
	if (!b_foundEnemyPath)
	{
		b_foundEnemyPath = SearchForPath();	//search path creates unitPath[] elements

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
			if ((character->getPos() - target->getPos()).Length() <= character->i_movementCost)
			{
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
					}
				}
			}
			else	//reached end of cost/limit allowed to move
			{
				state = IDLE;
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

