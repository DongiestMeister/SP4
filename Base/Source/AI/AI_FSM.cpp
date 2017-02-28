
#include "AI_FSM.h"
#include "../AStar/AStar.h"




AI_FSM::AI_FSM()
{
	character = nullptr;
	f_speed = 5;
	b_isDone = false;
	b_reachEnd = false;
	b_attack = false;
	b_foundEnemyPath = false;
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
	//targetVector.clear();
	int nearestDistance = INT_MAX;

	vector<vector<Vector2>> tempPath;

	//run list of units
	for (CharactersList::iterator it = map->characters.begin(); it != map->characters.end(); ++it)
	{
		
		
		Character* enemy = *it;
		//Check nearest target
		AStar search((int)character->getPos().x, (int)character->getPos().y, enemy->getPos().x, enemy->getPos().y, map);

		/*if ((character->getPos() - enemy->getPos()).Length() < character->i_movementCost
			&& (character->getPos() - enemy->getPos()).Length() < nearestDistance)*/
		if (search.Search())
		{
			if (search.bestPath.size() < nearestDistance)
			{
				targetVector.clear();
				tempPath.clear();
				targetVector.push_back(enemy);
				tempPath.push_back(search.bestPath);

				nearestDistance = search.bestPath.size();
			}
			//if both target is same distance
			else if (search.bestPath.size() == nearestDistance)
			{
				targetVector.push_back(enemy);
				tempPath.push_back(search.bestPath);
			}

		}
		
		/*else if ((character->getPos() - enemy->getPos()).Length() < character->i_movementCost
			&& (character->getPos() - enemy->getPos()).Length() == nearestDistance)
		{
			
		}*/
	}

	vector<Vector2> tempPath2;
	float maxHP = INT_MAX;
	for (int i = 0; i < targetVector.size(); ++i)
	{
		if (targetVector[i]->getCurrentHP() < maxHP)
		{
			targetVector.clear();
			maxHP = targetVector[i]->getCurrentHP();
			target = targetVector[i];
			tempPath2 = tempPath[i];
		}
	}

	SearchForPath(this->character->i_movementCost,tempPath2);
}

bool AI_FSM::SearchForPath(int move_cost,  vector<Vector2> searchPath)
{
	unitPath.clear();

	if (target) // != null
	{
		// Astar Search(curr pos , desired dist ,  )
		//AStar search((int)character->getPos().x, (int)character->getPos().y, (int)targetPosition.x, (int)targetPosition.y, map);
		//if (search.Search())//search.Search() = Finds the nearest path between parameters
		//{
			if (move_cost >= searchPath.size() - 1)
			{
				b_reachEnd = true;
				//std::cout << "1" << std::endl;
			}
		
			for (int i = 0; i < move_cost && i < searchPath.size() - 1; ++i)
			{
				unitPath.push_back(searchPath[i]);
				std::cout << "UnitPath : " << unitPath[i].x << " , " << unitPath[i].y << std::endl;
			}
			//std::cout << "UnitpAthSize : " << unitPath.size() << std::endl;
			std::cout << "new path" << std::endl;
			return true;

		//else
		//{
		//	return false;
		//}
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
	target = NULL;
	b_foundEnemyPath = false;
}

void AI_FSM::MoveUnit(double dt)
{
	// if dist btwn 1st pos & unit pos != 0
	if (unitPath.size() > 0)
	{
		if (!(unitPath[0] - character->getPos()).IsZero())
		{
			//find difference btwm unitPath and unitPos and store as temp
			Vector2 tempStep = (unitPath[0] - character->getPos()).Normalized();
			//add temp to curr unit pos for new pos
			character->setPos(character->getPos() + (tempStep *dt*f_speed));
			//std::cout << "loop1111" << std::endl;
		}
		//if value too low, snap character to "perfect" pos of unitPath
		if ((unitPath[0] - character->getPos()).Length() < 0.1f)
		{
			character->setPos(unitPath[0]);
			//delete 1st unitpath, unitPath[1] become unitPath[0]
			unitPath.erase(unitPath.begin());

			//????
			//std::cout << "loop22222" << std::endl;
		}
	}
}
