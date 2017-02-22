#ifndef AI_OFFENCE_H
#define AI_OFFENCE_H


#include "AI_FSM.h"


class AI_OffenceFSM : public AI_FSM
{
public:
	AI_OffenceFSM(Character *character);
	~AI_OffenceFSM();


	bool Update(double dt);

	enum Offence_Strategy
	{
		/*TO_GROUND_UNITS,
		TO_AIR_UNITS,

		SUPPORT_MEMBER,
		CAPTURE_POINT*/


		ATTACK,
		CHASE_ALL,
		CHASE_RANGED,
		CHASE_MELEE,
		CHASE_CAPTURETARGET,

	};

	Offence_Strategy state;

	vector<Vector2> unitPath;


	void Chase(double dt);



};

#endif