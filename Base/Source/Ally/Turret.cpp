#include "Turret.h"

CTurret::CTurret() 
	: activated(false)
	, setup(false)
	, headMovement(0.f)
	, Button(NULL)
	, Head(NULL)
	, Center(NULL)
	, Base(NULL)
{
	position.SetZero();
}

CTurret::~CTurret()
{
}

void CTurret::Init()
{
	Base = Create::Entity("Cylinder", position, Vector3(50, 10, 50));
	Base->InitLOD("Cylinder", "Cylinder_M", "Cylinder_L");
	CSceneNode *baseNode = CSceneGraph::GetInstance()->AddNode(Base);

	Center = Create::Entity("Cylinder", Vector3(0,50,0), Vector3(10, 100, 10));
	Center->InitLOD("Cylinder", "Cylinder_M", "Cylinder_L");
	CSceneNode *centerNode = baseNode->AddChild(Center);

	Head = Create::Entity("Cylinder", Vector3(0, -25, -25), Vector3(50, 50, 50));
	Head->InitLOD("Cylinder", "Cylinder_M", "Cylinder_L");
	CSceneNode *headNode = centerNode->AddChild(Head);
	headNode->ApplyRotate(90, 1, 0, 0);

	Button = Create::Entity("cube", Vector3(-27.5, 2.5, 0), Vector3(5, 5, 5));
	Button->SetCollider(true);
	Button->SetAABB(Button->GetScale() * 0.5, Button->GetScale() * -0.5);
	CSceneNode *buttonNode = baseNode->AddChild(Button);

	Sail = Create::Entity("Sail", Vector3(0, -32, 0), Vector3(5, 5, 5));
	Sail->InitLOD("Sail", "Sail_M", "Sail_L");
	CSceneNode *sailNode = headNode->AddChild(Sail);
	sailNode->ApplyRotate(-90, 0, 0, 1);
}

bool CTurret::IsActivated()
{
	return activated;
}

void CTurret::Activate()
{
	activated = true;
}

void CTurret::Update(double dt)
{
	CSceneNode *headNode = CSceneGraph::GetInstance()->GetNode(Head);
	CSceneNode *buttonNode = CSceneGraph::GetInstance()->GetNode(Button);
	CSceneNode *centerNode = CSceneGraph::GetInstance()->GetNode(Center);
	CSceneNode *sailNode = CSceneGraph::GetInstance()->GetNode(Sail);

	if (!buttonNode)
	{
		activated = true;
	}

	if (setup)
	{
		if (centerNode)
			centerNode->ApplyRotate(10 * dt, 0, 1, 0);
		if (sailNode)
			sailNode->ApplyRotate(30 * dt, 1, 0, 0);
	}
	else
	{
		if (activated && headNode)
		{
			if (headMovement < 90.f)
			{
				float translation = 10 * dt;
				headMovement += translation;
				if (headMovement > 90.f)
				{
					translation -= headMovement - 90.f;
					headMovement = 90.f;
					setup = true;
				}
				headNode->ApplyTranslate(0, 0, -translation);
			}
		}
	}
}

void CTurret::SetPosition(Vector3 pos)
{
	this->position = pos;
}

Vector3 CTurret::GetPosition()
{
	return position;
}