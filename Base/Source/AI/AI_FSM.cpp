
#include "AI_FSM.h"
#include "../AStar/AStar.h"




AI_FSM::AI_FSM()
{
	character = nullptr;
	f_speed = 5;
	b_isDone = false;
	b_reachEnd = false;
	b_attack = false;
}
AI_FSM::~AI_FSM()
{
	if (character)
	{
		//delete character; 
		character = nullptr;
	}
}


void AI_FSM::SearchNearestWithHP()
{
	vector<Character*> targetVector;
	//run list of units
	for (CharactersList::iterator it = map->characters.begin(); it != map->characters.end(); ++it)
	{
		float nearestDistance = FLT_MAX;

		Character* enemy = *it;
		//Check nearest target
		if ((character->getPos() - enemy->getPos()).Length() < character->i_movementCost
			&& (character->getPos() - enemy->getPos()).Length() <= nearestDistance)
		{
			targetVector.clear();
			targetVector.push_back(enemy);

			nearestDistance = (character->getPos() - enemy->getPos()).Length();
		}
		//if both target is same distance
		else if ((character->getPos() - enemy->getPos()).Length() < character->i_movementCost
			&& (character->getPos() - enemy->getPos()).Length() == nearestDistance)
		{
			targetVector.push_back(enemy);
		}
	}
	float maxHP = INT_MAX;
	for (int i = 0; i < targetVector.size(); ++i)
	{
		if (targetVector[i]->getCurrentHP() < maxHP)
		{
			maxHP = targetVector[i]->getCurrentHP();
			target = targetVector[i];
		}
	}


}

bool AI_FSM::SearchForPath()
{
	if (target) // != null
	{
		unitPath.clear();
		// Astar Search(curr pos , desired dist ,  )
		AStar search((int)character->getPos().x, (int)character->getPos().y, (int)target->getPos().x, (int)target->getPos().y, map);
		if (search.Search())//search.Search() = Finds the nearest path between parameters
		{
			if (character->i_movementCost >= search.bestPath.size() - 1)
			{
				b_reachEnd = true;
			}
			for (int i = 0; i < character->i_movementCost && i < search.bestPath.size() - 1; ++i)
			{
				unitPath.push_back(search.bestPath[i]);
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

void AI_FSM::Attack()
{

	PlayerInfo::GetInstance()->player = target;
	PlayerInfo::GetInstance()->enemy = this->character;

	PlayerInfo::GetInstance()->b_attacking = false;

	b_attack = true;
	b_isDone = true;
}

void AI_FSM::MoveUnit(double dt)
{
	// if dist btwn 1st pos & unit pos != 0
	if (!(unitPath[0] - character->getPos()).IsZero())
	{
		//find difference btwm unitPath and unitPos and store as temp
		Vector2 tempStep = (unitPath[0] - character->getPos()).Normalized();
		//add temp to curr unit pos for new pos
		character->setPos(character->getPos() + (tempStep *dt*f_speed));
	}
	//if value too low, snap character to "perfect" pos of unitPath
	if ((unitPath[0] - character->getPos()).Length() < 0.1f)
	{
		character->setPos(unitPath[0]);
		//delete 1st unitpath, unitPath[1] become unitPath[0]
		unitPath.erase(unitPath.begin());
	}
}
