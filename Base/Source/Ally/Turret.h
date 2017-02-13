#pragma once
#include "../Enemy/Enemy.h"

class Mesh;

class CTurret
{
protected:
	Vector3 position;

	float headMovement;

	bool setup;
	bool activated;
public:
	CTurret(void);
	virtual ~CTurret();

	void Init();
	// Reset this player instance to default
	void Reset(void);

	void Exit();

	void Activate();
	bool IsActivated();

	// Set position
	void SetPosition(Vector3 pos);
	Vector3 GetPosition();

	// Update
	void Update(double dt = 0.0333f);

	GenericEntity *Base,*Center,*Head, *Button, *Sail;
};
