
#include "DamageText.h"



DamageText::DamageText()
{

}

DamageText::~DamageText()
{

}


bool DamageText::Update(double dt)
{
	dmgTxt->SetPosition(dmgTxt->GetPosition() - Vector3(0, dt*15, 0));

	if (dmgTxt->GetPosition().y <= -30)
	{
		dmgTxt->SetIsDone(true);
		return true;
	}
	return false;

}
