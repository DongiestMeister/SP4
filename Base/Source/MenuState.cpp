#include <iostream>
using namespace std;

#include "MenuState.h"
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


CMenuState::CMenuState()
{
	
}

CMenuState::~CMenuState()
{

}

void CMenuState::Init()
{
	BGM = Music::GetInstance()->playSound("Sounds//badass.mp3", true, false, true);
	BGM->setVolume(0.3);

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//MenuState2.tga");

	MeshBuilder::GetInstance()->GenerateQuad("ARROW", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("ARROW")->textureID = LoadTGA("Image//arrowSwords.tga");

	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
	MenuStateBackground = Create::Sprite2DObject("MENUSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3(halfWindowWidth * 2, halfWindowHeight * 2, 0.0f));

	arrow = Create::Sprite2DObject("ARROW",
		Vector3(halfWindowWidth/2 +250, halfWindowHeight - 70, 0.1f),
		Vector3(100,100, 0.0f));
	cout << "CMenuState loaded\n" << endl;

	state = PLAY;

	float fontSize = 25.0f;
}

void CMenuState::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_UP))
	{
		Music::GetInstance()->playSound("Sounds//ding.wav");
		if (state == PLAY)
		{
			state = EXIT;
		}
		else
		{
			state = (MENU_STATE)(state - 1);
		}
		float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
		float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
		arrow->SetPosition(Vector3(halfWindowWidth / 2 + 250, halfWindowHeight - 70 - (int)state * 210, 0.1f));
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_DOWN))
	{
		Music::GetInstance()->playSound("Sounds//ding.wav");
		if (state == EXIT)
		{
			state = PLAY;
		}
		else
		{
			state = (MENU_STATE)(state + 1);
		}
		float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.f;
		float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.f;
		arrow->SetPosition(Vector3(halfWindowWidth / 2 + 250, halfWindowHeight - 70 - (int)state * 210, 0.1f));
	}

	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RETURN))
	{
		if (state == PLAY)
		{
			cout << "Loading LevelSelect" << endl;
			SceneManager::GetInstance()->SetActiveScene("GameState");
		}
		/*else if (state == OPTIONS)
		{
			cout << "Loading Optionstate" << endl;
			SceneManager::GetInstance()->SetActiveScene("OptionState");
		}
		else if (state == HIGHSCORES)
		{
			cout << "Loading Scorestate" << endl;
			SceneManager::GetInstance()->SetActiveScene("ScoreState");
		}*/
		else if (state == EXIT)
		{
			Application::GetInstance().ExitProgram = true;
		}
	}
}

void CMenuState::Render()
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

void CMenuState::Exit()
{
	BGM->stop();
	BGM->drop();
	EntityManager::GetInstance()->RemoveEntity(MenuStateBackground);
	EntityManager::GetInstance()->RemoveEntity(arrow);

	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");
	MeshBuilder::GetInstance()->RemoveMesh("ARROW");

	GraphicsManager::GetInstance()->DetachCamera();
}