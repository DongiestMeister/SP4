#include "GrenadeThrow.h"
#include "../Projectile/Grenade.h"
#include "../Sounds/Music.h"

CGrenadeThrow::CGrenadeThrow()
{
}


CGrenadeThrow::~CGrenadeThrow()
{
}

// Initialise this instance to default values
void CGrenadeThrow::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	totalRounds = 2;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 2;
	
	projectileSpeed = 100.f;

	// The time between shots
	timeBetweenShots = 1.0;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Discharge this weapon
void CGrenadeThrow::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire && !isReloading)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			CProjectile* aProjectile = Create::Grenade("grenade",
														position,
														(target - position).Normalized(),
														3.0f,
														8.0f,
														_source);
			aProjectile->SetCollider(false);
			bFire = false;
			Music::GetInstance()->playSound("Sounds//throwgrenade.wav");
			//magRounds--;
		}
	}
}

void CGrenadeThrow::Reload(void)
{
	
}