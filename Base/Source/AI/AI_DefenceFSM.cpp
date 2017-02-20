#include "AI_DefenceFSM.h"
#include "../AStar/AStar.h"
#include "SceneManager.h"

AI_DefenceFSM::AI_DefenceFSM(Character *character)
{
	this->character = character;
	target = nullptr;
	b_foundEnemyPath = false;
	state = IDLE;
	b_isDone = false;
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
			Idle();
			break;
		case CHASE:
			Chase(dt);
			break;
		case ATTACK:
			Attack();
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

void AI_DefenceFSM::Idle()
{
	
	//Set target of current unit to nearest target
	SearchNearestWithHP();
	
	if (target != nullptr)
	{
		//if (character->getHP() <= 20)
		//{
		//	state = RETREAT;
		//}
		//else
		{
			state = CHASE;
		}
	}
	else
	{
		b_isDone = true;
	}
}

void AI_DefenceFSM::Chase(double dt)
{
	
	if (!b_foundEnemyPath)
	{
		b_foundEnemyPath = SearchForPath();

		map->theScreenMap[unitPath[0].y][unitPath[0].x] = 0;
		map->theScreenMap[unitPath[unitPath.size() - 1].y][unitPath[unitPath.size() - 1].x] = 2;
	}

	//if unit != took action, = current unit to move
	if (b_foundEnemyPath)
	{
		if (!character->b_tookAction)
		{
			//DEFENCE CHASE = if target is out of reach, return to IDLE state
			if ((character->getPos() - target->getPos()).Length() <= character->i_movementCost)
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
			else
			{
				state = IDLE;
			}
		}
	}
}

void AI_DefenceFSM::Attack()
{

	PlayerInfo::GetInstance()->player = target;
	PlayerInfo::GetInstance()->enemy = this->character;

	PlayerInfo::GetInstance()->b_attacking = false;

	b_attack = true;
	state = IDLE;
	b_isDone = true;
}

void AI_DefenceFSM::Retreat()
{
	float nearestdist = FLT_MAX;
	
	for (CharactersList::iterator it = map->characters.begin(); it != map->characters.end(); ++it)
	{
		Character* enemy = *it;
		Character* ally = *it;

		//Only move when own unit is within range of the player attack = retreat
		if ((character->getPos() - enemy->getPos()).Length() <= enemy->i_movementCost)
		{
			if (((character->getPos() - ally->getPos()).Length()) < nearestdist)
			{
				nearestdist = ((character->getPos() - ally->getPos()).Length());
				target = ally;
			}
		}
	}
}

