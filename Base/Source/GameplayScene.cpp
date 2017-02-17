#include "GameplayScene.h"
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

#include "AI\AI_FSM.h"
#include "AI\AI_DefenceFSM.h"
#include "AI\AI_OffenceFSM.h"



#include <iostream>
using namespace std;


GameplayScene::GameplayScene()
{
}


GameplayScene::~GameplayScene()
{
}

void GameplayScene::Init()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	Music::GetInstance()->Init();

	turnDisplay = nullptr;
	b_textRunning = false;
	i_turn = 1;
	i_enemyIterator = 0;

	BGM = Music::GetInstance()->playSound("Sounds//bossfight.mp3", true, false, true);
	BGM->setVolume(0.3);

	srand(NULL);
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");
	

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, -10, 0);
	lights[0]->color.Set(1, 1, 0.7);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);
	
	// Create the playerinfo instance, which manages all information about the player
	//playerInfo = CPlayerInfo::GetInstance();
	//playerInfo->Init();

	//playerInfo->lives = 3;

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(Vector3(100,-10,100), Vector3(100,0,100), Vector3(0,0,1));
	camera.SetCameraOrtho(100.f, 16.f / 9.f);
	//playerInfo->AttachCamera(&camera);
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

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(100, 0, 100));
	groundEntity->SetScale(Vector3(750.0f, 750.0f, 750.0f));
	groundEntity->SetGrids(Vector3(1.0f, 1.0f, 1.0f));
	//playerInfo->SetTerrain(groundEntity);

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 5.0f;
	float halfFontSize = fontSize / 2.0f;

	fps = Create::Text2DObject("text", Vector3(-100,-95, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));
	//for (int i = 0; i < 6; ++i)
	//{
	//	textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	//}
	//textObj[0]->SetPosition(Vector3(halfWindowWidth * 0.75 - fontSize * 2, -halfWindowHeight * 0.75 + fontSize + halfFontSize, 0.0f));
	//textObj[0]->SetScale(Vector3(fontSize * 2, fontSize * 2, fontSize * 2));

	//textObj[3]->SetPosition(Vector3(-halfWindowWidth * 0.25 - fontSize * 2, halfWindowHeight * 0.75 + fontSize + halfFontSize, 0.0f));
	//textObj[3]->SetScale(Vector3(fontSize * 2, fontSize * 2, fontSize * 2));

	//textObj[4]->SetPosition(Vector3(-halfWindowWidth * 0.15 - fontSize * 2, halfWindowHeight * 0.65 + fontSize + halfFontSize, 0.0f));

	//textObj[5]->SetPosition(Vector3(-halfWindowWidth * 0.9 - fontSize * 2, -halfWindowHeight * 0.75 + fontSize + halfFontSize, 0.0f));
	//textObj[5]->SetScale(Vector3(fontSize * 2, fontSize * 2, fontSize * 2));

	controller.Init(&gameMap, &camera,&b_playerTurn);

	AStar search(0, 0, 9, 9, &gameMap);
	if (search.Search())
	{
		cout << "Search success" << endl;
	}
	else
	{
		cout << "Search failed" << endl;
	}

	Character *knight = new MeleeCharacter("K1");
	Character *knight1 = new MeleeCharacter("K2");
	knight1->FSM = new AI_DefenceFSM(knight1);
	knight1->FSM->map = &gameMap;
	Character *knight2 = new MeleeCharacter("K3");
	knight2->FSM = new AI_DefenceFSM(knight2);
	knight2->FSM->map = &gameMap;
	Character *knight3 = new MeleeCharacter("K4");
	knight3->FSM = new AI_DefenceFSM(knight3);
	knight3->FSM->map = &gameMap;
	//Unit *knight4 = new Unit(new MeleeCharacter());

	/*gameMap.AddCharacter(1, 1, knight);
	gameMap.AddCharacter(10, 15, knight1);
	gameMap.AddCharacter(4, 7, knight2);
	gameMap.AddCharacter(6, 5, knight3);
	gameMap.AddCharacter(8, 11, knight4);*/

	PlayerInfo::GetInstance()->addCharacterToParty(Vector2(1, 1), knight,1);
	PlayerInfo::GetInstance()->addCharacterToEnemies(Vector2(1, 3), knight1);
	PlayerInfo::GetInstance()->addCharacterToEnemies(Vector2(1, 2), knight2);
	PlayerInfo::GetInstance()->addCharacterToEnemies(Vector2(1, 4), knight3);

	gameMap.Init(200, 200, 10, 10); // Must be last line

	if (gameMap.LoadMap("Image//MapDesign.csv"))
	{
		cout << "Succesfully loaded map!" << endl;
	}

	DisplayText("Turn" + to_string(i_turn), Vector3(0, 1, 0));
}

void GameplayScene::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{

	}

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
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	//playerInfo->Update(dt);

	if (!b_textRunning)
	{
		if (b_playerTurn)
		{
			bool b_done = true;
			for (int i = 0; i < gameMap.characters.size(); ++i)
			{
				if (!gameMap.characters[i]->b_tookAction)
				{
					b_done = false;
				}
			}
			if (b_done)
			{
				b_playerTurn = false;
				gameMap.ResetCharacters();
				if (!b_textRunning)
					DisplayText("Enemy Turn", Vector3(1, 0, 0));
			}
		}
		else
		{
			bool b_done = true;
			for (int i = 0; i < gameMap.enemies.size(); ++i)
			{
				if (!gameMap.enemies[i]->b_tookAction)
				{
					b_done = false;
				}
			}
			if (b_done)
			{
				b_playerTurn = true;
				gameMap.ResetEnemies();
				i_turn++;
				controller.selectedTile = gameMap.characters[0]->getPos();
				if (!b_textRunning)
					DisplayText("Turn " + to_string(i_turn), Vector3(0, 1, 0));
			}
		}
	}

	if (b_textRunning)
	{	
		if (turnDisplay->GetPosition().x > 0 - 50 && turnDisplay->GetPosition().x < 0 - 40)
		{
			turnDisplay->SetPosition(turnDisplay->GetPosition() + Vector3(5, 0, 0) * dt);
		}
		else
		{
			turnDisplay->SetPosition(turnDisplay->GetPosition() + Vector3(100, 0, 0) * dt);
		}

		if (turnDisplay->GetPosition().x > 100)
		{
			turnDisplay->SetIsDone(true);
			b_textRunning = false;
		}
	}
	else
	{
		// Enemies movement

		if (!b_playerTurn)
		{
			controller.selectedTile = gameMap.enemies[i_enemyIterator]->getPos();
			if (gameMap.enemies[i_enemyIterator]->FSM && !gameMap.enemies[i_enemyIterator]->b_tookAction)
			{
				if (gameMap.enemies[i_enemyIterator]->FSM->Update(dt))
				{
					gameMap.enemies[i_enemyIterator]->b_tookAction = true;
					i_enemyIterator++;
					if (i_enemyIterator == gameMap.enemies.size())
					{
						i_enemyIterator = 0;
					}
				}
			}
			else
			{
				gameMap.enemies[i_enemyIterator]->b_tookAction = true;
				i_enemyIterator++;
				if (i_enemyIterator == gameMap.enemies.size())
				{
					i_enemyIterator = 0;
				}
			}
		}
	}

	controller.Update(dt);

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps2 = (float)(1.f / dt);
	ss << "FPS: " << fps2;
	//ss << "FPS: " << camera.GetCameraPos();
	fps->SetText(ss.str());

	//std::ostringstream ss1;
	//ss1.precision(4);
	//ss1 << "Player:" << playerInfo->GetPos();
	//textObj[2]->SetText(ss1.str());

	//ss1.str("");
	//ss1 << playerInfo->GetPrimaryWeapon()->GetMagRound() << "/" << playerInfo->GetPrimaryWeapon()->GetMaxMagRound();
	//textObj[0]->SetText(ss1.str());

	//if (playerInfo->lives > 0)
	//{
	//	ss1.str("");
	//	ss1 << "LIVES:" << (float)playerInfo->lives;
	//	textObj[3]->SetText(ss1.str());
	//	textObj[4]->SetText("");
	//}
	//else
	//{
	//	textObj[3]->SetText("GAME OVER");
	//	textObj[4]->SetText("(E TO RESTART)");
	//}

	//ss1.str("");
	//ss1 << "Score:" << m_iScore;
	//textObj[5]->SetText(ss1.str());
}

void GameplayScene::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Setup 3D pipeline then render 3D
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->SetOrthographicProjection(-camera.f_OrthoSize * camera.f_aspectRatio, camera.f_OrthoSize * camera.f_aspectRatio, -camera.f_OrthoSize, camera.f_OrthoSize, -2000, 2000);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	gameMap.Render();
	controller.Render();

	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;

	GraphicsManager::GetInstance()->SetOrthographicProjection(-100, 100, -100, 100, -2000, 2000);
	GraphicsManager::GetInstance()->DetachCamera();
	
	controller.RenderUI();
	EntityManager::GetInstance()->RenderUI();
}

void GameplayScene::Exit()
{
	// Detach camera from other entities
	BGM->drop();
	GraphicsManager::GetInstance()->DetachCamera();
	//playerInfo->DetachCamera();
	fps->SetIsDone(true);
//	if (playerInfo->DropInstance() == false)
//	{
//#if _DEBUGMODE==1
//		cout << "Unable to drop PlayerInfo class" << endl;
//#endif
//	}

	// Delete the lights
	//delete lights[0];
	//delete lights[1];
}

void GameplayScene::Pause()
{
	BGM->setIsPaused(true);
	groundEntity->SetIsDone(true);
	fps->SetIsDone(true);
	turnDisplay->SetIsDone(true);
	b_textRunning = false;
}

void GameplayScene::Resume()
{
	BGM->setIsPaused(false);

	controller.b_cameraTransition = false;
	camera.f_OrthoSize = controller.tempOrtho;

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
	// Customise the ground entity
	groundEntity->SetPosition(Vector3(100, 0, 100));
	groundEntity->SetScale(Vector3(750.0f, 750.0f, 750.0f));
	groundEntity->SetGrids(Vector3(1.0f, 1.0f, 1.0f));

	float fontSize = 5.0f;

	fps = Create::Text2DObject("text", Vector3(-100, -95, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));
}

void GameplayScene::DisplayText(string text,Vector3 color)
{
	b_textRunning = true;
	turnDisplay = Create::Text2DObject("text", Vector3(-120, 0, 1.0f), text, Vector3(10, 10, 10), Color(color.x, color.y, color.z));
}