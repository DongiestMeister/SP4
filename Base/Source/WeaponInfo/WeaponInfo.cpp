#include "WeaponInfo.h"
#include "../Projectile/Laser.h"
#include "../Sounds/Music.h"

#include <iostream>
using namespace std;

CWeaponInfo::CWeaponInfo()
	: magRounds(1)
	, maxMagRounds(1)
	, totalRounds(8)
	, maxTotalRounds(8)
	, timeBetweenShots(0.5)
	, elapsedTime(0.0)
	, bFire(true)
	, projectileSpeed(50.f)
	, mesh(NULL)
{
	rotation.SetZero();
	translation.SetZero();
	scale = Vector3(1, 1, 1);
	rotateOffset.SetZero();
	translateOffset.SetZero();
	reloadOffset = 0.f;
}


CWeaponInfo::~CWeaponInfo()
{
}

// Set the number of ammunition in the magazine for this player
void CWeaponInfo::SetMagRound(const int magRounds)
{
	this->magRounds = magRounds;
}

// Set the maximum number of ammunition in the magazine for this weapon
void CWeaponInfo::SetMaxMagRound(const int magRounds)
{
	this->maxMagRounds = magRounds;
}

// The current total number of rounds currently carried by this player
void CWeaponInfo::SetTotalRound(const int totalRounds)
{
	this->totalRounds = totalRounds;
}

// The max total number of rounds currently carried by this player
void CWeaponInfo::SetMaxTotalRound(const int maxTotalRounds)
{
	this->maxTotalRounds = maxTotalRounds;
}


// Get the number of ammunition in the magazine for this player
int CWeaponInfo::GetMagRound(void) const
{
	return magRounds;
}

// Get the maximum number of ammunition in the magazine for this weapon
int CWeaponInfo::GetMaxMagRound(void) const
{
	return maxMagRounds;
}

// Get the current total number of rounds currently carried by this player
int CWeaponInfo::GetTotalRound(void) const
{
	return totalRounds;
}

// Get the max total number of rounds currently carried by this player
int CWeaponInfo::GetMaxTotalRound(void) const
{
	return maxTotalRounds;
}

// Set the time between shots
void CWeaponInfo::SetTimeBetweenShots(const double timeBetweenShots)
{
	this->timeBetweenShots = timeBetweenShots;
}

// Set the firing rate in rounds per min
void CWeaponInfo::SetFiringRate(const int firingRate)
{
	timeBetweenShots = 60.0 / (double)firingRate;	// 60 seconds divided by firing rate
}

// Set the firing flag
void CWeaponInfo::SetCanFire(const bool bFire)
{
	this->bFire = bFire;
}

// Get the time between shots
double CWeaponInfo::GetTimeBetweenShots(void) const
{
	return timeBetweenShots;
}

// Get the firing rate
int CWeaponInfo::GetFiringRate(void) const
{
	return (int)(60.0 / timeBetweenShots);	// 60 seconds divided by timeBetweenShots
}

// Get the firing flag
bool CWeaponInfo::GetCanFire(void) const
{
	return bFire;
}

// Initialise this instance to default values
void CWeaponInfo::Init(void)
{
	// The number of ammunition in a magazine for this weapon
	magRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	totalRounds = 8;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 8;

	// The time between shots
	timeBetweenShots = 0.5;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
	// Shoot offset
	recoilOffset = 0.f;
	recoilLimit = 5.f;
	reloadOffset = 0.f;
	reloadLimit = 45.f;
	jumpOffset = 0.f;
	jumpLimit = 10.f;

	isReloading = false;
	weaponReloadingUp = false;
}

// Update the elapsed time
void CWeaponInfo::Update(const double dt)
{
	elapsedTime += dt;
	if (elapsedTime > timeBetweenShots)
	{
		bFire = true;
		elapsedTime = 0.0;
	}

	float rotateLimit = 3.f;
	float slow_speed = 10.f;

	// Update required for animations

	if (rotateOffset.y < -rotateLimit / 2)
	{
		rotateOffset.y = -rotateLimit / 2;
	}
	else if (rotateOffset.y > rotateLimit / 2)
	{
		rotateOffset.y = rotateLimit / 2;
	}
	else if(rotateOffset.y > 0)
	{
		rotateOffset.y = Math::Max(rotateOffset.y - slow_speed * (float)dt, 0.f);
	}
	else if (rotateOffset.y < 0)
	{
		rotateOffset.y = Math::Min(rotateOffset.y + slow_speed * (float)dt, 0.f);
	}

	if (rotateOffset.x < -rotateLimit)
	{
		rotateOffset.x = -rotateLimit;
	}
	else if (rotateOffset.x > rotateLimit)
	{
		rotateOffset.x = rotateLimit;
	}
	else if (rotateOffset.x > 0)
	{
		rotateOffset.x = Math::Max(rotateOffset.x - slow_speed * (float)dt, 0.f);
	}
	else if (rotateOffset.x < 0)
	{
		rotateOffset.x = Math::Min(rotateOffset.x + slow_speed * (float)dt, 0.f);
	}

	// Jumping offset

	if (jumpOffset < -jumpLimit)
	{
		jumpOffset = -jumpLimit;
	}
	else if (jumpOffset > jumpLimit)
	{
		jumpOffset = jumpLimit;
	}
	else if (jumpOffset > 0)
	{
		jumpOffset = Math::Max(jumpOffset - slow_speed * (float)dt, 0.f);
	}
	else if (jumpOffset < 0)
	{
		jumpOffset = Math::Min(jumpOffset + slow_speed * (float)dt, 0.f);
	}

	if (recoilOffset > 0)
	{
		recoilOffset = Math::Max(0.f, recoilOffset - slow_speed * (float)dt);
	}

	float reloadSpeed = 90.f;
	if (isReloading)
	{
		if (weaponReloadingUp)
		{
			if (reloadOffset < reloadLimit)
			{
				reloadOffset += reloadSpeed * dt;
			}
			else
			{
				if (magRounds < maxMagRounds)
				{
					magRounds = maxMagRounds;
				}
				weaponReloadingUp = false;
			}
		}
		else
		{
			if (reloadOffset > 0)
			{
				reloadOffset -= reloadSpeed * dt;
				if (reloadOffset < 0)
				{
					reloadOffset = 0.f;
					isReloading = false;
				}
			}
		}

	}


}

// Discharge this weapon
void CWeaponInfo::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire && !isReloading)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			CLaser* aProjectile = Create::Laser("sphere", 
															position + (target - position) * 5.f,
															(target - position).Normalized(), 
															5.f,
															30.0f, 
															projectileSpeed,
															_source);
			aProjectile->SetScale(Vector3(0.5f, 0.5f, 0.5f));
			aProjectile->SetCollider(true);
			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
			recoilOffset = recoilLimit;
			Music::GetInstance()->playSound("Sounds//pistol.mp3");
		}
	}
	else
	{
		int a = 0;
	}
}

// Reload this weapon
void CWeaponInfo::Reload(void)
{
	if (!isReloading && magRounds != maxMagRounds)
	{
		isReloading = true;
		weaponReloadingUp = true;
		Music::GetInstance()->playSound("Sounds//pistolreload.wav");
	}
}

// Add rounds
void CWeaponInfo::AddRounds(const int newRounds)
{
	if (totalRounds + newRounds > maxTotalRounds)
		totalRounds = maxTotalRounds;
	else
		totalRounds += newRounds;
}

// Print Self
void CWeaponInfo::PrintSelf(void)
{
	cout << "CWeaponInfo::PrintSelf()" << endl;
	cout << "========================" << endl;
	cout << "magRounds\t\t:\t" << magRounds << endl;
	cout << "maxMagRounds\t\t:\t" << maxMagRounds << endl;
	cout << "totalRounds\t\t:\t" << totalRounds << endl;
	cout << "maxTotalRounds\t\t:\t" << maxTotalRounds << endl;
	cout << "timeBetweenShots\t:\t" << timeBetweenShots << endl;
	cout << "elapsedTime\t\t:\t" << elapsedTime << endl;
	cout << "bFire\t\t:\t" << bFire << endl;
}
