#include "AI_OffenceFSM.h"
#include "../AStar/AStar.h"

AI_OffenceFSM::AI_OffenceFSM()
{

}


AI_OffenceFSM::~AI_OffenceFSM()
{

}

void AI_OffenceFSM::Update(double dt)
{

	if (!character->b_tookAction)
	{
		switch (state)
		{
		case ATTACK:
			Attack(); 
			break;
		case CHASE:
			Chase(dt);
			break;



		default:
			break;
		}
	}
}


void AI_OffenceFSM::Attack()
{
	
}

void AI_OffenceFSM::Chase(double dt)
{
	//Set target to current unit that is nearest
	SearchNearestWithHP();

	//if unit != took action, = current unit to move
	if (!character->b_tookAction)
	{
		//ATTACK CHASE = if target is out of reach, Keep chasing

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
}
	
