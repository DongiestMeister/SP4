#include <iostream>
using namespace std;

#include "IntroState.h"
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

CIntroState::CIntroState()
{

}

CIntroState::~CIntroState()
{

}

void CIntroState::Init()
{
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

	MeshBuilder::GetInstance()->GenerateQuad("INTROSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BKGROUND")->textureID = LoadTGA("Image//IntroState.tga");
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
	IntroStateBackground = Create::Sprite2DObject("INTROSTATE_BKGROUND",
										Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
										Vector3(halfWindowWidth * 2, halfWindowHeight * 2, 0.0f));
	cout << "CIntroState loaded\n" << endl;

	timer = 3.f;

	Music::GetInstance()->playSound("Sounds//Tether_target.mp3");
}

void CIntroState::Update(double dt)
{
	if ((timer -= dt) <= 0)
	{
		cout << "Loading Menustate" << endl;
	//	SceneManager::GetInstance()->SetActiveScene("GameState");
		SceneManager::GetInstance()->SetActiveScene("PartySelect");
	}
}

void CIntroState::Render()
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

void CIntroState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(IntroStateBackground);

	MeshBuilder::GetInstance()->RemoveMesh("INTROSTATE_BKGROUND");

	GraphicsManager::GetInstance()->DetachCamera();
}