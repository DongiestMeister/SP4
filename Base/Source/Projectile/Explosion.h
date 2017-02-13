#pragma once
#include "EntityBase.h"
#include "Vector3.h"

class Mesh;

class CExplosion : public EntityBase
{
public:
	CExplosion(void);
	CExplosion(Mesh* _modelMesh);
	~CExplosion(void);
	virtual void Update(double dt = 0.0333f);
	virtual void Render(void);
	void SetLifetime(float m_fLifetime);
protected:
	float m_fLifetime;
	Mesh* modelMesh;
};

namespace Create
{
	CExplosion* Explosion(const std::string& _meshName,
		const Vector3& _position,
		const float m_fLifetime);
};