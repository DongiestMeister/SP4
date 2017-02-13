#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Sounds/Music.h"

CEnemy::CEnemy()
	: defaultPosition(Vector3(0.0f,0.0f,0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, Torso(NULL)
	, Head(NULL)
	, LArm(NULL)
	, RArm(NULL)
	, LLeg(NULL)
	, RLeg(NULL)
	, torsoNode(NULL)
	, headNode(NULL)
	, LArmNode(NULL)
	, RArmNode(NULL)
	, LLegNode(NULL)
	, RLegNode(NULL)
	, rotation(0)
	, isDone(false)
	, Walking(true)
	, walkingSpeed(80.f)
	, reverseCountdown(0.f)
	, m_dSpeed(25.f)
	, hasFallen(false)
	, fallHeight(0.f)
	, waypointCounter(0)
	, player(nullptr)
	, attackTimer(2.f)
{
}


CEnemy::~CEnemy()
{
	Exit();
}

void CEnemy::Init(void)
{
	rotation = 0.f;
	state = PATROL;
	waypointCounter = 0;

	// Set the default values
	defaultPosition.Set(0, -7.5, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	walkingSpeed = 80.f;
	reverseCountdown = 0.f;

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Init body parts

	Center = Create::Entity("cube", position);
	Center->SetCollider(false);
	centerNode = CSceneGraph::GetInstance()->AddNode(Center);

	Torso = Create::Entity("cube", Vector3(0,0,0), Vector3(6, 6, 6));
	Torso->SetCollider(true);
	Torso->SetAABB(Torso->GetScale() * 0.5, Torso->GetScale() * -0.5);
	//Torso->InitLOD("cube", "cube", "cube");
	torsoNode = centerNode->AddChild(Torso);

	View = Create::Entity("cube", Vector3(1,0,0),Vector3(0.1,0.1,0.1));
	View->SetCollider(false);
	View->SetLODStatus(View->NO_DETAILS);
	viewNode = torsoNode->AddChild(View);

	Head = Create::Entity("Head", Vector3(0,4.5,0), Vector3(3, 3, 3));
	Head->SetCollider(true);
	Head->SetAABB(Head->GetScale() * 0.5, Head->GetScale() * -0.5);
	Head->InitLOD("Head", "Head_M", "Head_L");
	headNode = torsoNode->AddChild(Head);

	LArm = Create::Entity("cubeSG", Vector3(0,0,4.5), Vector3(4.5, 6, 3));
	LArm->SetCollider(true);
	LArm->SetAABB(LArm->GetScale() * 0.5, LArm->GetScale() * -0.5);
	//LArm->InitLOD("cubeSG", "cubeSG", "cubeSG");

	LArmNode = torsoNode->AddChild(LArm);


	RArm = Create::Entity("cubeSG", Vector3(0, 0, -4.5), Vector3(4.5, 6, 3));
	RArm->SetCollider(true);
	RArm->SetAABB(RArm->GetScale() * 0.5, RArm->GetScale() * -0.5);
	//RArm->InitLOD("cubeSG", "cubeSG", "cubeSG");

	RArmNode = torsoNode->AddChild(RArm);

	LLeg = Create::Entity("cubeSG", Vector3(0, -6, 1.5), Vector3(3, 6, 3));
	LLeg->SetCollider(true);
	LLeg->SetAABB(LLeg->GetScale() * 0.5, LLeg->GetScale() * -0.5);
	//LLeg->InitLOD("cubeSG", "cubeSG", "cubeSG");

	LLegNode = torsoNode->AddChild(LLeg);


	RLeg = Create::Entity("cubeSG", Vector3(0, -6, -1.5), Vector3(3, 6, 3));
	RLeg->SetCollider(true);
	RLeg->SetAABB(RLeg->GetScale() * 0.5, RLeg->GetScale() * -0.5);
	//RLeg->InitLOD("cubeSG", "cubeSG", "cubeSG");

	RLegNode = torsoNode->AddChild(RLeg);
}

// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CEnemy::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

// Update
void CEnemy::Update(double dt)
{
	viewNode = CSceneGraph::GetInstance()->GetNode(View);
	centerNode = CSceneGraph::GetInstance()->GetNode(Center);
	torsoNode = CSceneGraph::GetInstance()->GetNode(Torso);
	headNode = CSceneGraph::GetInstance()->GetNode(Head);
	LArmNode = CSceneGraph::GetInstance()->GetNode(LArm);
	RArmNode = CSceneGraph::GetInstance()->GetNode(RArm);
	LLegNode = CSceneGraph::GetInstance()->GetNode(LLeg);
	RLegNode = CSceneGraph::GetInstance()->GetNode(RLeg);

	//Vector3 viewVector = (target - position).Normalized();
	//Vector3 velocity = viewVector * (float)m_dSpeed * (float)dt;
	//position += velocity;


	if (!headNode && torsoNode)
	{
		EntityManager::GetInstance()->MarkForDeletion(Center);
		Music::GetInstance()->play3DSound("Sounds//ouch.wav", position.x, position.y, position.z);
		return;
	}
	else if (!torsoNode)
	{
		Music::GetInstance()->play3DSound("Sounds//ouch.wav", position.x, position.y, position.z);
		return;
	}

	if (Walking)
	{
		float deltaRotation = walkingSpeed * dt;

		reverseCountdown += walkingSpeed * dt;
		if (reverseCountdown >= 45.f || reverseCountdown <= -45.f)
		{
			walkingSpeed = -walkingSpeed;
			if (reverseCountdown > 45.f)
			{
				deltaRotation -= (reverseCountdown - 45.f);
				reverseCountdown = 45.f;
			}
			else if (reverseCountdown < -45.f)
			{
				deltaRotation -= (reverseCountdown + 45.f);
				reverseCountdown = -45.f;
			}
		}

		if (LArmNode)
		{
			if (LArmNode->GetParent() == torsoNode)
				LArmNode->ApplyRotate(-deltaRotation, 0, 0, 1);
		}

		if (RArmNode)
		{
			if (RArmNode->GetParent() == torsoNode)
				RArmNode->ApplyRotate(deltaRotation, 0, 0, 1);
		}

		if (LLegNode)
		{
			if (LLegNode->GetParent() == torsoNode)
				LLegNode->ApplyRotate(deltaRotation, 0, 0, 1);
		}

		if (RLegNode)
		{
			if (RLegNode->GetParent() == torsoNode)
				RLegNode->ApplyRotate(-deltaRotation, 0, 0, 1);
		}
	}

	Vector3 direction = target - Torso->GetPosition();
	direction.y = 0;
	Vector3 view = View->GetPosition() - Torso->GetPosition();
	view.y = 0;
	float tempRotation = Math::RadianToDegree(acos(direction.Dot(view) / direction.Length()));
	float thing = (-view.z*direction.x) + (view.x*direction.z);
	if (thing < 0)
	{
		tempRotation = -tempRotation;
	}
	if (tempRotation > 5)
	{
		torsoNode->ApplyRotate(-100 * dt, 0, 1, 0);
	}
	else if (tempRotation < -5)
	{
		torsoNode->ApplyRotate(100 * dt, 0, 1, 0);
	}

	switch (state)
	{
	case PATROL:
		Patrol(dt);
		break;
	case CHASE:
		Chase(dt);
		break;
	case ATTACK:
		Attack(dt);
		break;
	}


	// Constrain the position
	//Constrain();

	if (!RLegNode && !LLegNode && !hasFallen)
	{
		float falldist = 10.f * (float)dt;
		fallHeight -= falldist;
		if (fallHeight < -5)
		{
			hasFallen = true;
			falldist -= fallHeight + 5;
		}
		torsoNode->ApplyTranslate(0, -falldist, 0);
	}



	// Update the target
	position = Center->GetPosition();
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CEnemy::Render(void)
{

}

void CEnemy::Exit()
{
	CSceneNode* centerNode = CSceneGraph::GetInstance()->GetNode(Center);
	if (centerNode)
	{
		centerNode->GetEntity()->SetIsDone(true);
	}
}

void CEnemy::AddWaypoint(Vector3 pos)
{
	waypoints.push_back(pos);
}

void CEnemy::GoTo(Vector3 target, double dt)
{
	if (centerNode)
	{
		Vector3 direction = (Vector3(target.x, 0, target.z) - Vector3(position.x, 0, position.z)).Normalized();
		direction = direction * m_dSpeed * dt;
		centerNode->ApplyTranslate(direction.x, direction.y, direction.z);
	}
}

void CEnemy::Patrol(double dt)
{
	if ((this->position - player->GetPos()).LengthSquared() < 150 * 150)
	{
		state = CHASE;
		return;
	}

	if (waypoints.size() > 0)
	{
		target = waypoints.at(waypointCounter);
	}

	if (torsoNode)
	{
		if ((Vector3(target.x, 0, target.z) - Vector3(position.x, 0, position.z)).LengthSquared() > 2)
		{
			GoTo(target, dt);
			Walking = true;
		}
		else
		{
			Walking = false;
			if (waypoints.size() > 0)
			{
				if (target == waypoints.at(waypointCounter))
				{
					waypointCounter++;
					if (waypointCounter >= waypoints.size())
					{
						waypointCounter = 0;
					}
				}
			}

		}
		//torsoNode->ApplyTranslate(m_dSpeed * dt, 0, 0);
	}
}

void CEnemy::Chase(double dt)
{
	target = player->GetPos();

	if ((Vector3(target.x, 0, target.z) - Vector3(position.x, 0, position.z)).LengthSquared() > 170 * 170)
	{
		state = PATROL;
		return;
	}

	if (torsoNode)
	{
		if ((Vector3(target.x, 0, target.z) - Vector3(position.x, 0, position.z)).LengthSquared() > 30 * 30)
		{
			GoTo(target, dt);
			Walking = true;
		}
		else
		{
			Walking = false;
			state = ATTACK;
		}
		//torsoNode->ApplyTranslate(m_dSpeed * dt, 0, 0);
	}
}

void CEnemy::Attack(double dt)
{
	target = player->GetPos();
	if ((Vector3(target.x, 0, target.z) - Vector3(position.x, 0, position.z)).LengthSquared() > 50 * 50)
	{
		state = CHASE;
		return;
	}

	if (player->lives > 0)
	{
		if ((attackTimer -= dt) <= 0)
		{
			attackTimer = 2.f;
			player->lives--;
		}
	}
	else
	{
		state = PATROL;
	}
}