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

	MeshBuilder::GetInstance()->GenerateQuad("ARROW", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("ARROW")->textureID = LoadTGA("Image//arrow.tga");

	MeshBuilder::GetInstance()->GenerateQuad("ARROW2", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("ARROW2")->textureID = LoadTGA("Image//arrow2.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
	OptionStateBackground = Create::Sprite2DObject("OPTIONSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(halfWindowWidth * 2, halfWindowHeight * 2, 0.0f));
	cout << "COptionState loaded\n" << endl;

	arrow[0] = Create::Sprite2DObject("ARROW2",
		Vector3(halfWindowWidth - 300, halfWindowHeight - 25, 0.1f),
		Vector3(100, 100, 0.0f));

	arrow[1] = Create::Sprite2DObject("ARROW",
		Vector3(halfWindowWidth + 300, halfWindowHeight - 25, 0.1f),
		Vector3(100, 100, 0.0f));
	arrow[1]->SetRotation(Vector3(180, 0, 0));

	float fontSize = 50.0f;

	state = AUDIO;

	textObj = Create::Text2DObject("text", Vector3(halfWindowWidth / 2 + 100.f, halfWindowHeight + 200, 0.1f), "AUDIO", Vector3(fontSize, fontSize, fontSize), Color(1.f, 1.f, 1.f));
}

void COptionState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
	{
		cout << "Loading Menustate" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_LEFT))
	{
		Music::GetInstance()->playSound("Sounds//ding.wav");
		if (state == AUDIO)
		{
			state = INPUT;
		}
		else
		{
			state = (OPTIONS)(state - 1);
		}
		switch (state)
		{
		case AUDIO:
			textObj->SetText("AUDIO");
			break;
		case VIDEO:
			textObj->SetText("VIDEO");
			break;
		case INPUT:
			textObj->SetText("INPUT");
			break;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RIGHT))
	{
		Music::GetInstance()->playSound("Sounds//ding.wav");
		if (state == INPUT)
		{
			state = AUDIO;
		}
		else
		{
			state = (OPTIONS)(state + 1);
		}
		switch (state)
		{
		case AUDIO:
			textObj->SetText("AUDIO");
			break;
		case VIDEO:
			textObj->SetText("VIDEO");
			break;
		case INPUT:
			textObj->SetText("INPUT");
			break;
		}
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
}

void COptionState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(OptionStateBackground);

	EntityManager::GetInstance()->RemoveEntity(textObj);

	EntityManager::GetInstance()->RemoveEntity(arrow[0]);
	EntityManager::GetInstance()->RemoveEntity(arrow[1]);

	MeshBuilder::GetInstance()->RemoveMesh("ARROW");
	MeshBuilder::GetInstance()->RemoveMesh("ARROW2");
	MeshBuilder::GetInstance()->RemoveMesh("OPTIONSTATE_BKGROUND");

	GraphicsManager::GetInstance()->DetachCamera();
}