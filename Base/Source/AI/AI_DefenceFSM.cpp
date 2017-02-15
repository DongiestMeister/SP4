#include "AI_DefenceFSM.h"
#include "../AStar/AStar.h"

AI_DefenceFSM::AI_DefenceFSM()
{
	target = nullptr;
}

AI_DefenceFSM::~AI_DefenceFSM()
{

}

void AI_DefenceFSM::Update(double dt)
{
	if (!character->b_tookAction)
	{
		switch (state)
		{
		case IDLE:
			Idle();
			break;
		case CHASE:
			Chase();
			break;
		case ATTACK:
			Attack();
			break;
		default:
			break;
		}
	}
}

void AI_DefenceFSM::Idle()
{
	float nearestDistance = FLT_MAX;
	for (vector<Unit*>::iterator it = map->characters.begin(); it != map->characters.end(); ++it)
	{
		Unit* enemy = *it;
		if ((character->getPos() - enemy->character->getPos()).Length() < character->i_movementCost
			&& (character->getPos() - enemy->character->getPos()).Length() < nearestDistance)
		{
			target = enemy;
			nearestDistance = (character->getPos() - enemy->character->getPos()).Length();
		}
	}

	if (target != nullptr)
	{
		state = CHASE;
	}
}

void AI_DefenceFSM::Chase()
{
	
}

void AI_DefenceFSM::Attack()
{
	//
}


bool AI_DefenceFSM::SearchPath()
{
	if (target)
	{
		AStar search((int)character->getPos().x, (int)character->getPos().y, (int)target->character->getPos().x, (int)target->character->getPos().y, map);
		if (search.Search())//search.Search() = Finds the nearest path between parameters
		{
			for (int i = 0; i < character->i_movementCost + 1; ++i)
			{
				if (search.bestPath.size() < i)
				{
					unitPath.push_back(search.bestPath[i]);
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}