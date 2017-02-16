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
			Chase(dt);
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
	
	//Set target of current unit to nearest target
	SearchNearestWithHP();
	

	if (target != nullptr)
	{
		if (character->getHP() <= 20)
		{
			state = RETREAT;
		}
		else
		{
			state = CHASE;
		}
	}
}

void AI_DefenceFSM::Chase(double dt)
{

	//if unit != took action, = current unit to move
	if (!character->b_tookAction)
	{
		//DEFENCE CHASE = if target is out of reach, return to IDLE state
		if ((character->getPos() - target->getPos()).Length() <= character->i_movementCost)
		{
			//search through paths from unitpath
			for (int i = 0; i < unitPath.size(); ++i)
			{
				// if dist btwn 1st pos & unit pos != 0, 
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
				if ((unitPath.size() <= 0))
				{
					state = ATTACK;
				}
			}
		}
		else
		{
			state = IDLE;
		}
	}
}

void AI_DefenceFSM::Attack()
{
	//
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

