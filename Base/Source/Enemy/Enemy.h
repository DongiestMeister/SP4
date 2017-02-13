#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../SceneGraph/SceneGraph.h"
#include "../PlayerInfo/PlayerInfo.h"

class Mesh;

class CEnemy
{
protected:
	enum AI_STATE
	{
		PATROL,
		CHASE,
		ATTACK,
		AI_TOTAL
	};

	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	Vector3 position;

	double m_dSpeed;
	double m_dAcceleration;
	float walkingSpeed;
	bool Walking;
	float reverseCountdown;

	float rotation;

	bool hasFallen;
	float fallHeight;

	vector<Vector3> waypoints;
	AI_STATE state;

	void GoTo(Vector3 target,double dt);

	void Patrol(double dt);
	void Chase(double dt);
	void Attack(double dt);

	float attackTimer;
public:
	CEnemy(void);
	virtual ~CEnemy();

	void Init(void);
	// Reset this player instance to default
	void Reset(void);

	void Exit();

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);

	bool isDone;

	float walkRotationCheck;

	GenericEntity *Torso, *Head, *LArm, *RArm, *LLeg, *RLeg;
	CSceneNode *torsoNode, *headNode, *LArmNode, *RArmNode, *LLegNode, *RLegNode;
	GenericEntity *Center,*View;
	CSceneNode *centerNode,*viewNode;

	void AddWaypoint(Vector3 pos);

	int waypointCounter;

	CPlayerInfo *player;
};
