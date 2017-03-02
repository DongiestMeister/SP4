#include <iostream>
using namespace std;

#include "OptionState.h"
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

COptionState::COptionState()
{

}

COptionState::~COptionState()
{

}

void COptionState::Init()
{
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("OPTIONSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("OPTIONSTATE_BKGROUND")->textureID = LoadTGA("Image//OptionState.tga");

	MeshBuilder::GetInstance()->GenerateQuad("yellowsquare", Color(1, 1, 0), 1.f);

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
	OptionStateBackground = Create::Sprite2DObject("OPTIONSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(halfWindowWidth * 2, halfWindowHeight * 2, 0.0f));
	//cout << "COptionState loaded\n" << endl;

}

void COptionState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased('X'))
	{
		//cout << "Loading Menustate" << endl;
		SceneManager::GetInstance()->SetActiveScene("WarMap");
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_LEFT))
	{
		if (Music::GetInstance()->theSoundEngine->getSoundVolume() > 0)
		{
			Music::GetInstance()->theSoundEngine->setSoundVolume(Music::GetInstance()->theSoundEngine->getSoundVolume() - 0.1f);
		}
		Music::GetInstance()->playSound("Sounds//ding.wav");
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RIGHT))
	{
		Music::GetInstance()->theSoundEngine->setSoundVolume(Music::GetInstance()->theSoundEngine->getSoundVolume() + 0.1f);
		if (Music::GetInstance()->theSoundEngine->getSoundVolume() > 1)
		{
			Music::GetInstance()->theSoundEngine->setSoundVolume(1);
		}
		Music::GetInstance()->playSound("Sounds//ding.wav");
	}
}

void COptionState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.f, 4.f / 3.f, 0.1f, 10000.f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	EntityManager::GetInstance()->Render();

	GraphicsManager::GetInstance()->SetOrthographicProjection(0,
		Application::GetInstance().GetWindowWidth(),
		0,
		Application::GetInstance().GetWindowHeight(),
		-10, 10);

	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();

	MS &modelStack = GraphicsManager::GetInstance()->GetModelStack();

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;

	RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Volume " + to_string((int)((Music::GetInstance()->theSoundEngine->getSoundVolume() + 0.005f) * 100)) + "%", Vector3(halfWindowWidth - 45, halfWindowHeight * 1.2f, 1), 80, Color(1, 1, 0));

	for (float i = 0; i < Music::GetInstance()->theSoundEngine->getSoundVolume(); i += 0.1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(halfWindowWidth / 2 + (i * halfWindowWidth), halfWindowHeight, 1);
		modelStack.Scale(halfWindowWidth / 10, halfWindowHeight / 10, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("yellowsquare"));
		modelStack.PopMatrix();
	}
}

void COptionState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(OptionStateBackground);

	MeshBuilder::GetInstance()->RemoveMesh("OPTIONSTATE_BKGROUND");

	GraphicsManager::GetInstance()->DetachCamera();
}