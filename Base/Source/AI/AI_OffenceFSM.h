#ifndef AI_OFFENCE_H
#define AI_OFFENCE_H


#include "AI_FSM.h"


class AI_OffenceFSM : public AI_FSM
{
public:
	AI_OffenceFSM();
	~AI_OffenceFSM();


	void Update(double dt);

	enum Offence_Strategy
	{
		/*TO_GROUND_UNITS,
		TO_AIR_UNITS,

		SUPPORT_MEMBER,
		CAPTURE_POINT*/


		ATTACK,
		CHASE


	};

	Offence_Strategy state;

	vector<Vector2> unitPath;


	void Attack();
	void Chase(double dt);



};

#endif