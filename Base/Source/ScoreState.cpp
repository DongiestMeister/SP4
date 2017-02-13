#include <iostream>
using namespace std;

#include "ScoreState.h"
#include "GL/glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "TextEntity.h"
#include "RenderHelper.h"
#include "SpriteEntity.h"
#include "EntityManager.h"

#include "KeyboardController.h"
#include "SceneManager.h"

#include "Lua\LuaInterface.h"

CScoreState::CScoreState()
{

}

CScoreState::~CScoreState()
{

}

void CScoreState::Init()
{
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("SCORESTATE_BKGROUND", Color(1, 1, 1), 1.f);

	MeshBuilder::GetInstance()->GetMesh("SCORESTATE_BKGROUND")->textureID = LoadTGA("Image//HighScores.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
	ScoreStateBackground = Create::Sprite2DObject("SCORESTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(halfWindowWidth * 2, halfWindowHeight * 2, 0.0f));
	cout << "CScoreState loaded\n" << endl;

	float fontSize = 50.0f;

	vector<int> temp = CLuaInterface::GetInstance()->GetHighscores();

	textObj[0] = Create::Text2DObject("text", Vector3(halfWindowWidth/2, halfWindowHeight, 0.1f), "1)Score:" + to_string(temp[0]), Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));
	textObj[1] = Create::Text2DObject("text", Vector3(halfWindowWidth/2, halfWindowHeight - 100, 0.1f), "2)Score:" + to_string(temp[1]), Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));
	textObj[2] = Create::Text2DObject("text", Vector3(halfWindowWidth/2, halfWindowHeight - 200, 0.1f), "3)Score:" + to_string(temp[2]), Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));
}

void CScoreState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_BACK))
	{
		cout << "Loading Menustate" << endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
}

void CScoreState::Render()
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

void CScoreState::Exit()
{
	EntityManager::GetInstance()->RemoveEntity(ScoreStateBackground);

	EntityManager::GetInstance()->RemoveEntity(textObj[0]);
	EntityManager::GetInstance()->RemoveEntity(textObj[1]);
	EntityManager::GetInstance()->RemoveEntity(textObj[2]);

	MeshBuilder::GetInstance()->RemoveMesh("SCORESTATE_BKGROUND");

	GraphicsManager::GetInstance()->DetachCamera();
}