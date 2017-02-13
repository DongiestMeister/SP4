#include "Pistol.h"
#include "../Application.h"


CPistol::CPistol()
{
}


CPistol::~CPistol()
{
}

// Initialise this instance to default values
void CPistol::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 8;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 8;
	// The current total number of rounds currently carried by this player
	totalRounds = 40;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 40;

	projectileSpeed = 300.f;

	// The time between shots
	timeBetweenShots = 0.3333;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	scale.x = Application::GetInstance().GetWindowWidth() / 3;
	scale.y= Application::GetInstance().GetWindowWidth() / 3;
	scale.z = Application::GetInstance().GetWindowWidth() / 3;

	rotation.y = 105.f;
	rotation.x = 5.f;
	rotation.z = 0.f;

	translation.x = Application::GetInstance().GetWindowWidth() / 4;
	translation.y = -Application::GetInstance().GetWindowWidth() / 3;
}
