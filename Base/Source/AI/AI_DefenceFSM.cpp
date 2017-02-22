#include "AI_DefenceFSM.h"
#include "../AStar/AStar.h"
#include "SceneManager.h"
#include "../PlayerInfo.h"

AI_DefenceFSM::AI_DefenceFSM(Character *character)
{
	this->character = character;
	target = nullptr;
	b_foundEnemyPath = false;
	state = IDLE;
	b_isDone = false;
	originalPosition = (character->getPos().x, character->getPos().y);
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
			Idle(false);
			break;
		case CHASE_DEF:
			Chase(dt);
			break;
		case ATTACK:
			Attack();
			break;
		case STATIONARY:
			Stationary(dt);
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

void AI_DefenceFSM::Idle(bool is_stationary)
{

	//Set target of current unit to nearest target
	SearchNearestWithHP();

	if (!is_stationary)
	{
		if (target != nullptr)
		{
			if (character->getHP() <= 20)
			{
				state = RETREAT;
			}
			else
			{
				state = CHASE_DEF;
			}
		}
	}
	else if (is_stationary)
	{
		state = STATIONARY;
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
		b_foundEnemyPath = SearchForPath();
		if (b_foundEnemyPath)
		{
			map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
			map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;
		}



	if (b_foundEnemyPath)
	{
		//if unit != took action, = current unit to move
		if (!character->b_tookAction)
		{
			//STATIONARY CHASE = hardcoded fixed values so units dont move out of defending range
			if ((character->getPos() - target->getPos()).Length() <= 2)
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
					}
				}
			}
			else if (!(character->getPos() == originalPosition))
			{
				/*ReturnToPosition(dt);*/
				state = RETURN_TO_POSITION;
			}
			else	//reached end of cost/limit allowed to move
			{
				state = IDLE;
				b_isDone = true;
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
		if ((character->getPos() - originalPosition).Length() <= 2)
		{

			MoveUnit(dt);

			if (unitPath.size() <= 0)
			{
				if (b_reachEnd)
				{
					b_reachEnd = false;
					state = IDLE;
				}
				else
				{
					b_isDone = true;
				}
			}
		}
		else
		{
			state = IDLE;
			b_isDone = true;
		}
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

