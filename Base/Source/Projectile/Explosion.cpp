#include "Explosion.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

CExplosion::CExplosion()
{
}

CExplosion::CExplosion(Mesh* _modelMesh)
{
	this->modelMesh = _modelMesh;
}

CExplosion::~CExplosion()
{

}

void CExplosion::Update(double dt)
{
	m_fLifetime -= (float)dt;
	if (m_fLifetime > 0)
	{
		this->scale = scale + Vector3(300, 300, 300) * dt;
	}
	else
	{
		this->SetIsDone(true);
	}
}

void CExplosion::SetLifetime(float m_fLifetime)
{
	this->m_fLifetime = m_fLifetime;
}

void CExplosion::Render()
{
	if (m_fLifetime < 0.0f)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

CExplosion* Create::Explosion(const std::string& _meshName,
	const Vector3& _position,
	const float m_fLifetime)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CExplosion* result = new CExplosion(modelMesh);
	result->SetPosition(_position);
	result->SetLifetime(m_fLifetime);
	EntityManager::GetInstance()->AddEntity(result, false);

	return result;
}