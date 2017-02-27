#include <iostream>

#include "RecruitScene.h"
#include "GL/glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "TextEntity.h"
#include "RenderHelper.h"
#include "SpriteEntity.h"
#include "EntityManager.h"
#include "Sounds\Music.h"

#include "KeyboardController.h"
#include "SceneManager.h"

RecruitScene::RecruitScene()
{

}

RecruitScene::~RecruitScene()
{

}

void RecruitScene::Init()
{
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("summonMenu", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("summonMenu")->textureID = LoadTGA("Image//RecruitScene//summonMenu.tga");
	MeshBuilder::GetInstance()->GenerateQuad("newUnit", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("newUnit")->textureID = LoadTGA("Image//RecruitScene//newUnit.tga");

	b_showObtainedUnit = false;
	newChar = nullptr;
	message = "";
	attackType = "";
}

void RecruitScene::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased('Z'))
	{
		if (PlayerInfo::GetInstance()->gold >= 100)
		{
			if (PlayerInfo::GetInstance()->availableUnits.size() < 15)
			{
				if (b_showObtainedUnit == false)
				{
					b_showObtainedUnit = true;
					PlayerInfo::GetInstance()->gold -= 100;

					int result = Math::RandIntMinMax(0, PlayerInfo::GetInstance()->unitsNotOwned.size() - 1);
					newChar = PlayerInfo::GetInstance()->unitsNotOwned.at(result);
					if (newChar->i_attackRange == 1)
						attackType = "Melee";
					else if (newChar->i_attackRange == 2)
						attackType = "Ranged";
					PlayerInfo::GetInstance()->addCharacter(Vector2(0, 0), newChar);
					PlayerInfo::GetInstance()->unitsNotOwned.erase(PlayerInfo::GetInstance()->unitsNotOwned.begin() + result);
				}
			}
			else
				message = "Cannot obtain any more units!";
		}
		else
			message = "Not Enough Gold!";
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('X'))
	{
		if (b_showObtainedUnit == true)
		{
			b_showObtainedUnit = false;
		}
		else
		{
			SceneManager::GetInstance()->SetActiveScene("WarMap");
		}
	}
}

void RecruitScene::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.f, 4.f / 3.f, 0.1f, 10000.f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();

	GraphicsManager::GetInstance()->SetOrthographicProjection(-100,100,-100,100,-2000,2000);

	GraphicsManager::GetInstance()->DetachCamera();

	float screenwidth = Application::GetInstance().GetWindowWidth();
	float screenheight = Application::GetInstance().GetWindowHeight();
	EntityManager::GetInstance()->RenderUI();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -1);
	modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("summonMenu"));
	modelStack.PopMatrix();

	RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), message, Vector3(-30, 50, 2), 15.f, Color(1, 1, 1));
	RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->gold), Vector3(10, 79, 2), 12.f, Color(1, 1, 0));
	RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.size()) + "/15", Vector3(70, 79, 2), 12.f, Color(1, 1, 1));

	if (b_showObtainedUnit)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -10, 1);
		modelStack.Scale(90, 90 * 16 / 9, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("newUnit"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(20, 25, 2);
		modelStack.Scale(20, 20 * 16 / 9, 1);
		RenderHelper::RenderMesh(newChar->getPortrait());
		modelStack.PopMatrix();

		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), newChar->getName(), Vector3(-20, 30, 2), 15.f, Color(1, 1, 1));
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), attackType, Vector3(-20, -20, 2), 15.f, Color(1, 1, 1));
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(newChar->i_movementCost), Vector3(-20, -65, 2), 15.f, Color(1, 1, 1));

		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Press X", Vector3(-87, -50, 2), 15.f, Color(1, 1, 0));
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "to go back", Vector3(-87, -65, 2), 15.f, Color(1, 1, 0));
	}

	glEnable(GL_CULL_FACE);
}

void RecruitScene::Exit()
{

	GraphicsManager::GetInstance()->DetachCamera();
}