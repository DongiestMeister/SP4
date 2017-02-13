#include "GenericEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "SceneGraph\SceneGraph.h"

GenericEntity::GenericEntity(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
	if (position.x > 1000 || position.x < -1000 || position.z > 1000 || position.z < -1000)
	{
		isDone = true;
	}
}

void GenericEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();

	if (CSceneGraph::GetInstance()->GetNode(this))
	{
		modelStack.MultMatrix(CSceneGraph::GetInstance()->GetNode(this)->transformation_Matrix);
		modelStack.Scale(scale.x, scale.y, scale.z);
	}
	else
	{
		modelStack.Translate(position.x, position.y, position.z);
		modelStack.Rotate(rotate.z, 0, 0, 1);
		modelStack.Rotate(rotate.x, 1, 0, 0);
		modelStack.Rotate(rotate.y, 0, 1, 0);
		modelStack.Scale(scale.x, scale.y, scale.z);
	}


	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
			RenderHelper::RenderMeshWithLight(GetLODMesh());
	}
	else
		RenderHelper::RenderMeshWithLight(modelMesh);
	modelStack.PopMatrix();
}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

GenericEntity* Create::Entity(	const std::string& _meshName, 
								const Vector3& _position,
								const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}

GenericEntity* Create::Asset(	const std::string& _meshName,
								const Vector3& _position,
								const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	return result;
}

