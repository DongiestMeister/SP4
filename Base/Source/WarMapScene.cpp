#include "WarMapScene.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "RenderHelper.h"

#include <iostream>
using namespace std;


WarMapScene::WarMapScene()
{
}


WarMapScene::~WarMapScene()
{

}

void WarMapScene::Init()
{
	Math::InitRNG();

	BGM = Music::GetInstance()->playSound("Sounds//bossfight.mp3", true, false, true);
	BGM->setVolume(0.3);

	country = SINGAPORE;
	currentButton = PLAY;

	// Create and attach the camera to the scene
	camera.Init(Vector3(100, -10, 100), Vector3(100, 0, 100), Vector3(0, 0, 1));
	camera.SetCameraOrtho(100.f, 16.f / 9.f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//Grassydirt.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//Sunset_Front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//Sunset_Back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//Sunset_Left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//Sunset_Right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//Sunset_Top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//Sunset_Bottom.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 1.f);

	MeshBuilder::GetInstance()->GenerateQuad("Selected", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Selected")->textureID = LoadTGA("Image//selected.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SelectedArrow", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectedArrow")->textureID = LoadTGA("Image//selectarrow.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Knight", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Knight")->textureID = LoadTGA("Image//knight.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Attack", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Attack")->textureID = LoadTGA("Image//attack.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Move", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Move")->textureID = LoadTGA("Image//move.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Wait", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Wait")->textureID = LoadTGA("Image//wait.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Forest", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Forest")->textureID = LoadTGA("Image//forest.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Win", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Win")->textureID = LoadTGA("Image//youwin.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Frame", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Frame")->textureID = LoadTGA("Image//frame.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Singapore", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Singapore")->textureID = LoadTGA("Image//singapore.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Ocean", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Ocean")->textureID = LoadTGA("Image//ocean.tga");

	MeshBuilder::GetInstance()->GenerateQuad("PlayButton", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("PlayButton")->textureID = LoadTGA("Image//gotowar.tga");

	MeshBuilder::GetInstance()->GenerateQuad("OptionButton", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("OptionButton")->textureID = LoadTGA("Image//optionsbutton.tga");

	MeshBuilder::GetInstance()->GenerateQuad("RecruitButton", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("RecruitButton")->textureID = LoadTGA("Image//recruit.tga");

	MeshBuilder::GetInstance()->GenerateQuad("TransparentBack", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("TransparentBack")->textureID = LoadTGA("Image//transparentback.tga");

	MeshBuilder::GetInstance()->GenerateQuad("RedSphere", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("RedSphere")->textureID = LoadTGA("Image//redsphere.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GreenSphere", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GreenSphere")->textureID = LoadTGA("Image//greensphere.tga");

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 5.0f;
	float halfFontSize = fontSize / 2.0f;

	fps = Create::Text2DObject("text", Vector3(-100, -95, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));
	select = Create::Sprite2DObject("SelectedArrow", Vector3(50, 50, 0.5), Vector3(40, 40, 40));

	f_buttonXoffset = 0.f;

	RandomLevels();
	levelIterator = 0;
}

void WarMapScene::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	MouseControl(dt);

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps2 = (float)(1.f / dt);
	ss << "FPS: " << fps2;
	//ss << "FPS: " << camera.GetCameraPos();
	fps->SetText(ss.str());

	switch (currentButton)
	{
	case PLAY:
		select->SetPosition(Vector3(70 + f_buttonXoffset, 50, 0.5));
		select->SetScale(Vector3(40, 40, 40));
		break;
	case OPTIONS:
		select->SetPosition(Vector3(70 + f_buttonXoffset, 0, 0.5));
		select->SetScale(Vector3(40, 40, 40));
		break;
	case RECRUIT:
		select->SetPosition(Vector3(70 + f_buttonXoffset, -50, 0.5));
		select->SetScale(Vector3(40, 40, 40));
		break;
	case B_TOTAL:
		if (levelList.size() > levelIterator)
		{
			select->SetPosition(Vector3(levelList[levelIterator].pos.x,levelList[levelIterator].pos.y,0.5));
			select->SetScale(Vector3(9, 16, 1));
		}
		break;
	}
}

void WarMapScene::MouseControl(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		//cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		//cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		//cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		//cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		//cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('Z'))
	{
		if (currentButton == PLAY)
		{
			currentButton = B_TOTAL;
		}

		else if (currentButton == B_TOTAL)
		{
			if (!levelList[levelIterator].b_completed)
			{
				PlayerInfo::GetInstance()->level = &levelList[levelIterator];
				SceneManager::GetInstance()->SetActiveScene("PartySelect", true);
			}
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('X'))
	{
		if (currentButton == B_TOTAL)
		{
			currentButton = PLAY;
		}
	}

	if (currentButton == B_TOTAL) // if a button has been selected
	{
		if (f_buttonXoffset < 50)
		{
			f_buttonXoffset += 100 * dt;
		}
		else
		{
			f_buttonXoffset = 50.f;
		}
	}
	else
	{
		if (f_buttonXoffset > 0)
		{
			f_buttonXoffset -= 100 * dt;
		}
		else
		{
			f_buttonXoffset = 0.f;
		}
	}

	if (currentButton != B_TOTAL)
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
		{
			if (currentButton == PLAY)
			{
				currentButton = (BUTTONS)(B_TOTAL - 1);
			}
			else
			{
				currentButton = (BUTTONS)(currentButton - 1);
			}
		}

		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
		{
			if (currentButton == (BUTTONS)(B_TOTAL - 1))
			{
				currentButton = PLAY;
			}
			else
			{
				currentButton = (BUTTONS)(currentButton + 1);
			}
		}
	}

	if (currentButton == B_TOTAL) // if a button has been selected
	{
		if (f_buttonXoffset < 50)
		{
			f_buttonXoffset += 100 * dt;
		}
		else
		{
			f_buttonXoffset = 50.f;
		}
	}
	else
	{
		if (f_buttonXoffset > 0)
		{
			f_buttonXoffset -= 100 * dt;
		}
		else
		{
			f_buttonXoffset = 0.f;
		}
	}

	if (currentButton == B_TOTAL) // Controlling levels
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT) || KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
		{
			levelIterator++;
			if (levelIterator >= levelList.size())
			{
				levelIterator = 0;
			}
		}

		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT) || KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
		{
			if (levelIterator == 0)
			{
				levelIterator = levelList.size() - 1;
			}
			else
			{
				levelIterator--;
			}
		}
	}
}

void WarMapScene::Render()
{
	MS &modelStack = GraphicsManager::GetInstance()->GetModelStack();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Setup 3D pipeline then render 3D
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->SetOrthographicProjection(-camera.f_OrthoSize * camera.f_aspectRatio, camera.f_OrthoSize * camera.f_aspectRatio, -camera.f_OrthoSize, camera.f_OrthoSize, -2000, 2000);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D

	GraphicsManager::GetInstance()->SetOrthographicProjection(-100, 100, -100, 100, -2000, 2000);
	GraphicsManager::GetInstance()->DetachCamera();
	//
	//if (b_renderWin)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate(winPos.x, winPos.y, 3);
	//	modelStack.Scale(100, 70, 1);
	//	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Win"));
	//	modelStack.PopMatrix();
	//}

	modelStack.PushMatrix();
	modelStack.Translate(-15, 0, 0.1);
	modelStack.Scale(160, 200, 200);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Singapore"));
	modelStack.PopMatrix();
	RenderBackground();
	RenderLevels();
	RenderButtons();

	EntityManager::GetInstance()->RenderUI();
}

void WarMapScene::Exit()
{
	// Detach camera from other entities
	BGM->stop();
	BGM->drop();
	BGM = nullptr;

	//playerInfo->DetachCamera();
	//fps->SetIsDone(true);
	EntityManager::GetInstance()->RemoveEntity(select);
	EntityManager::GetInstance()->RemoveEntity(fps);
	//	if (playerInfo->DropInstance() == false)
	//	{
	//#if _DEBUGMODE==1
	//		cout << "Unable to drop PlayerInfo class" << endl;
	//#endif
	//	}

	// Delete the lights
	//delete lights[0];
	//delete lights[1];
	//GraphicsManager::GetInstance()->RemoveLight("lights[0]");
	//GraphicsManager::GetInstance()->RemoveLight("lights[1]");
}

void WarMapScene::Pause()
{
	BGM->setIsPaused(true);
	EntityManager::GetInstance()->RemoveEntity(fps);
	EntityManager::GetInstance()->RemoveEntity(select);
	fps = nullptr;
}

void WarMapScene::Resume()
{
	BGM->setIsPaused(false);

	float fontSize = 5.0f;

	select = Create::Sprite2DObject("SelectedArrow", Vector3(50, 50, 0.4), Vector3(40, 40, 40));
	fps = Create::Text2DObject("text", Vector3(-100, -95, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));
}

void WarMapScene::RenderBackground()
{
	MS &modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(200, 200, 200);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Ocean"));
	modelStack.PopMatrix();
}

void WarMapScene::RenderButtons()
{
	MS &modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(70 + f_buttonXoffset, 50, 0.4);
	modelStack.Scale(40, 40, 40);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("PlayButton"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(70 + f_buttonXoffset, 0, 0.4);
	modelStack.Scale(40, 40, 40);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("OptionButton"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(70 + f_buttonXoffset, -50, 0.4);
	modelStack.Scale(40, 40, 40);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("RecruitButton"));
	modelStack.PopMatrix();

	// Selection thing

	modelStack.PushMatrix();
	modelStack.Translate(70 + f_buttonXoffset, 0, 0.3);
	modelStack.Scale(80, 200, 40);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("TransparentBack"));
	modelStack.PopMatrix();
}

void WarMapScene::RandomLevels()
{
	int numberOfLevels = Math::RandIntMinMax(4, 6);

	float distance = 20;
	for (int i = 0; i < numberOfLevels; ++i)
	{
		Level tempLevel;

		int randomType = Math::RandIntMinMax(1, 3);
		if (i >= 0 && i < 3)
		{
			randomType = i;
		}
		if (randomType == 0)
		{
			tempLevel.condition = KILL;
		}
		else if (randomType == 1)
		{
			tempLevel.condition = SURVIVE;
		}
		else if (randomType == 2)
		{
			tempLevel.condition = CAPTURE;
		}

		tempLevel.pos = Vector2(-100 + distance, Math::RandIntMinMax(-50, 50));
		distance += 160 / numberOfLevels;
		levelList.push_back(tempLevel);
	}
}

void WarMapScene::RenderLevels()
{
	MS &modelStack = GraphicsManager::GetInstance()->GetModelStack();

	for (unsigned int i = 0; i < levelList.size(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(levelList[i].pos.x, levelList[i].pos.y, 0.2);
		modelStack.Scale(9, 16, 10);
		if (!levelList[i].b_win)
		{
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("RedSphere"));
		}
		else
		{
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GreenSphere"));
		}
		modelStack.PopMatrix();

		if (i == levelIterator && currentButton == B_TOTAL)
		{
			string winCondition = "Win condition:";
			if (levelList[i].condition == KILL)
			{
				winCondition += "Kill All Enemies";
			}
			else if (levelList[i].condition == SURVIVE)
			{
				winCondition += "Survive";
			}
			else if (levelList[i].condition == CAPTURE)
			{
				winCondition += "Capture";
			}
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), winCondition, Vector3(levelList[i].pos.x, levelList[i].pos.y + 20, 0.2), 5.f, Color(1, 1, 1));
			if (levelList[i].b_completed)
			{
				if (levelList[i].b_win)
				{
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "You won!", Vector3(levelList[i].pos.x, levelList[i].pos.y + 15, 0.2), 5.f, Color(1, 1, 1));
				}
				else
				{
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "You lost!", Vector3(levelList[i].pos.x, levelList[i].pos.y + 15, 0.2), 5.f, Color(1, 1, 1));
				}
			}
			else
			{
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Contested point", Vector3(levelList[i].pos.x, levelList[i].pos.y + 15, 0.2), 5.f, Color(1, 1, 1));
			}
		}
	}
}