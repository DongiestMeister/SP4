
#include "AI_FSM.h"
#include "../AStar/AStar.h"




AI_FSM::AI_FSM()
{
	character = nullptr;
	f_speed = 5;
	b_isDone = false;
}
AI_FSM::~AI_FSM()
{
	if (character)
	{
		//delete character; 
		character = nullptr;
	}
}

//void AI_FSM::SearchNearest()
//{
//	//run list of units
//	for (vector<Unit*>::iterator it = map->characters.begin(); it != map->characters.end(); ++it)
//	{
//		float nearestDistance = FLT_MAX;
//
//		Unit* enemy = *it;
//		//Get length between own unit and player position, 
//		if ((character->getPos() - enemy->character->getPos()).Length() < character->i_movementCost
//			&& (character->getPos() - enemy->character->getPos()).Length() < nearestDistance)
//		{
//			target = enemy;
//			//if dist < own movecost & less than max dist
//			//replace "newest" max dist
//			nearestDistance = (character->getPos() - enemy->character->getPos()).Length();
//		}
//	}
//}
//
//
//void AI_FSM::SearchLowestHP()
//{
//	//run through enemy list
//	for (vector<Unit*>::iterator it = map->characters.begin(); it != map->characters.end(); ++it)
//	{
//		Unit* enemy = *it;
//		int maxhealth = INT_MAX;
//
//		//if get target with next lowest hp, update maxhealth target
//		if (enemy->character->getHP() <= maxhealth)
//		{
//			target = enemy;
//			maxhealth = enemy->character->getHP();
//		}
//	}
//}

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
		/*else if ((character->getPos() - enemy->character->getPos()).Length() < character->i_movementCost
			&& (character->getPos() - enemy->character->getPos()).Length() == nearestDistance)
		{
			targetVector.push_back(enemy);
		}*/
	}
	float maxHP = INT_MAX;
	for (int i = 0; i < targetVector.size(); ++i)
	{
		if (targetVector[i]->getHP() < maxHP)
		{
			maxHP = targetVector[i]->getHP();
			target = targetVector[i];
		}
	}


}

bool AI_FSM::SearchPath()
{
	if (target) // != null
	{
		unitPath.clear();
		// Astar Search(curr pos , desired dist ,  )
		AStar search((int)character->getPos().x, (int)character->getPos().y, (int)target->getPos().x, (int)target->getPos().y, map);
		if (search.Search())//search.Search() = Finds the nearest path between parameters
		{
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

bool AI_FSM::SearchPathWithTerrain()
{
	if (target)
	{
		return true;
	}
}