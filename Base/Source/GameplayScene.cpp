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
	currProg = GraphicsManager::GetInstance()->GetActiveShader();

	b_renderQuit = false;
	turnDisplay = nullptr;
	b_textRunning = false;
	i_turn = 1;
	i_enemyIterator = 0;
	b_FastForward = false;

	f_timeDelay = 1.f;

	BGM = Music::GetInstance()->playSound("Sounds//bossfight.mp3", true, false, true);
	BGM->setVolume(0.3);

	srand(NULL);
	
	// Tell the graphics manager to use the shader we just loaded

	/*lights[0] = new Light();
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
	currProg->UpdateInt("textEnabled", 0);*/
	
	// Create and attach the camera to the scene
	camera.Init(Vector3(100,-10,100), Vector3(100,0,100), Vector3(0,0,1));
	camera.SetCameraOrtho(100.f, 16.f / 9.f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	//MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	//MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
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

	MeshBuilder::GetInstance()->GenerateQuad("RedSelected", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("RedSelected")->textureID = LoadTGA("Image//redselected.tga");

	MeshBuilder::GetInstance()->GenerateQuad("SelectedArrow", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SelectedArrow")->textureID = LoadTGA("Image//selectarrow.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Knight", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Knight")->textureID = LoadTGA("Image//knight.tga");

	MeshBuilder::GetInstance()->GenerateQuad("BlueKnight", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BlueKnight")->textureID = LoadTGA("Image//blueknight.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Attack", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Attack")->textureID = LoadTGA("Image//attack.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Special", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Special")->textureID = LoadTGA("Image//special.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Move", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Move")->textureID = LoadTGA("Image//move.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Wait", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Wait")->textureID = LoadTGA("Image//wait.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Forest", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Forest")->textureID = LoadTGA("Image//forest.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Flag", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Flag")->textureID = LoadTGA("Image//flag.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Win", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Win")->textureID = LoadTGA("Image//youwin.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Lose", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Lose")->textureID = LoadTGA("Image//youlose.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Boulder", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Boulder")->textureID = LoadTGA("Image//boulder.tga");

	MeshBuilder::GetInstance()->GenerateQuad("RockFloor", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("RockFloor")->textureID = LoadTGA("Image//rockfloor.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Quitting", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Quitting")->textureID = LoadTGA("Image//quitting.tga");

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(200, 0, 200));
	groundEntity->SetScale(Vector3(2500.0f, 2500.0f, 2500.0f));
	groundEntity->SetGrids(Vector3(1.0f, 1.0f, 1.0f));
	//playerInfo->SetTerrain(groundEntity);

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 5.0f;
	float halfFontSize = fontSize / 2.0f;

	//fps = Create::Text2DObject("text", Vector3(-100,-95, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));
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
	spawner.Init(&gameMap);

	if (PlayerInfo::GetInstance()->level)
	{
		spawner.LoadEnemies(PlayerInfo::GetInstance()->level->s_enemyType);
		spawner.LoadSpawns(PlayerInfo::GetInstance()->level->s_enemySpawn);


		gameMap.Init(400, 400, PlayerInfo::GetInstance()->level->i_mapSizeX, PlayerInfo::GetInstance()->level->i_mapSizeY, &spawner);

		if (gameMap.LoadMap(PlayerInfo::GetInstance()->level->s_mapName))
		{
			//cout << "Succesfully loaded map!" << endl;
		}


		condition = PlayerInfo::GetInstance()->level->condition;
		if (condition == SURVIVE)
		{
			i_surviveTurns = PlayerInfo::GetInstance()->level->i_surviveTurns;
			DisplayText("Survive " + to_string(i_surviveTurns) + " turns", Vector3(0, 1, 0));
		}
		else if (condition == CAPTURE)
		{
			DisplayText("Capture the flag!", Vector3(0, 1, 0));
		}
		else
		{
			DisplayText("Kill all enemies!", Vector3(0, 1, 0));
		}
	}
	controller.selectedTile = gameMap.characters[0]->getPos();
	b_renderWin = false;
	b_renderLose = false;
	b_playerTurn = true;
}

void GameplayScene::Update(double dt)
{
	// Update our entities

	if (b_FastForward)
	{
		dt *= 1.5f;
	}

	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	LightMouseControl(dt);
	// <THERE>
	gameMap.Update(dt);

	if (!b_renderWin && !b_renderLose)
	{
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
					if (condition == CAPTURE)
					{
						Vector2 pos = gameMap.characters[i]->getPos();
						if (gameMap.GetObstacle(pos.x, pos.y).type == 5)
						{
							DisplayWin(true);
						}
					}
				}
				if (b_done)
				{
					b_playerTurn = false;
					gameMap.ResetCharacters();
					i_enemyIterator = 0;
					spawner.SpawnWave(i_turn);
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
					controller.b_forceCamera = false;
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
				turnDisplay->SetPosition(turnDisplay->GetPosition() + Vector3(150, 0, 0) * dt);
			}

			if (turnDisplay->GetPosition().x > 100)
			{
				EntityManager::GetInstance()->RemoveEntity(turnDisplay);
				turnDisplay = nullptr;
				b_textRunning = false;
			}
		}
		else
		{
			// Enemies movement
			if (!b_playerTurn)
			{
				if (!controller.b_cameraTransition)
				{
					controller.selectedTile = gameMap.enemies[i_enemyIterator]->getPos();
				}
				controller.b_forceCamera = true;
				if ((f_timeDelay -= dt) < 0)
				{
					if (gameMap.enemies[i_enemyIterator]->FSM && !gameMap.enemies[i_enemyIterator]->b_tookAction)
					{
						if (!controller.b_cameraTransition)
						{
							if (gameMap.enemies[i_enemyIterator]->FSM->Update(dt))
							{
								if (gameMap.enemies[i_enemyIterator]->FSM->b_attack)
								{
									gameMap.enemies[i_enemyIterator]->FSM->b_attack = false;
									controller.b_cameraTransition = true;
									camera.SetCameraPos(Vector3(controller.selectedTile.x * gameMap.tileSizeX + gameMap.tileSizeX / 2, camera.GetCameraPos().y, controller.selectedTile.y * gameMap.tileSizeY + gameMap.tileSizeY / 2));
									camera.SetCameraTarget(Vector3(controller.selectedTile.x * gameMap.tileSizeX + gameMap.tileSizeX / 2, camera.GetCameraTarget().y, controller.selectedTile.y * gameMap.tileSizeY + gameMap.tileSizeY / 2));
									if (PlayerInfo::GetInstance()->player)
									{
										controller.selectedTile = PlayerInfo::GetInstance()->player->getPos();
									}
								}
								else
								{
									gameMap.enemies[i_enemyIterator]->b_tookAction = true;
								}
							}
						}
					}
					else
					{
						gameMap.enemies[i_enemyIterator]->b_tookAction = true;
						i_enemyIterator++;
						f_timeDelay = 1.f;
						if (i_enemyIterator == gameMap.enemies.size())
						{
							i_enemyIterator = 0;
						}
					}
				}
			}
		}

		controller.Update(dt);
	}

	if (condition == KILL && (!b_renderWin && !b_renderLose))
	{
		if (gameMap.enemies.size() == 0)
		{
			DisplayWin(true);
		}
		else if (gameMap.characters.size() == 0)
		{
			DisplayWin(false);
		}
	}
	else if (condition == SURVIVE && (!b_renderWin && !b_renderLose))
	{
		if (i_turn > i_surviveTurns)
		{
			DisplayWin(true);
		}
		else if (gameMap.characters.size() == 0)
		{
			DisplayWin(false);
		}
	}
	else if (gameMap.characters.size() == 0)
	{
		DisplayWin(false);
	}


	

	if ((b_renderWin || b_renderLose) && bannerPos.y > 0)
	{
		bannerPos = bannerPos + Vector2(0, -100 * dt);
	}

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	//std::ostringstream ss;
	//ss.precision(5);
	//float fps2 = (float)(1.f / dt);
	//ss << "FPS: " << fps2;
	////ss << "FPS: " << camera.GetCameraPos();
	//fps->SetText(ss.str());

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

void GameplayScene::LightMouseControl(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		b_renderQuit = true;
	}

	if (KeyboardController::GetInstance()->IsKeyDown('F'))
	{
		b_FastForward = true;
	}
	else
	{
		b_FastForward = false;
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('Z'))
	{
		if (b_renderWin)
		{
			SceneManager::GetInstance()->SetActiveScene("WarMap");
			PlayerInfo::GetInstance()->level->b_completed = true;
			PlayerInfo::GetInstance()->level->b_win = true;
		}
		else if (b_renderLose)
		{
			SceneManager::GetInstance()->SetActiveScene("WarMap");
			PlayerInfo::GetInstance()->level->b_completed = true;
			PlayerInfo::GetInstance()->level->b_win = false;
		}
		else if (b_renderQuit)
		{
			SceneManager::GetInstance()->SetActiveScene("WarMap");
			PlayerInfo::GetInstance()->level->b_completed = true;
			PlayerInfo::GetInstance()->level->b_win = false;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('X'))
	{
		if (b_renderQuit)
		{
			b_renderQuit = false;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		if (gameMap.characters.size() > 0)
		{
			gameMap.RemovePlayer(gameMap.characters[0]);
		}
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('K'))
	{
		if (controller.selectedUnit)
			controller.selectedUnit->i_specialMeter = 100;
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		//cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		//cout << "Right Mouse Button was released!" << endl;
		DisplayWin(false);
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		//cout << "Middle Mouse Button was released!" << endl;
		DisplayWin(true);
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		//cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		//cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
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

	GraphicsManager::GetInstance()->SetOrthographicProjection(-100, 100, -100, 100, -2000, 2000);
	GraphicsManager::GetInstance()->DetachCamera(); 

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	//
	if (b_renderWin)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bannerPos.x, bannerPos.y, 3);
		modelStack.Scale(100, 70, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Win"));
		modelStack.PopMatrix();
	}
	else if (b_renderLose)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bannerPos.x, bannerPos.y, 3);
		modelStack.Scale(100, 70, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Lose"));
		modelStack.PopMatrix();
	}
	else if (b_renderQuit)
	{
		modelStack.PushMatrix();
		modelStack.Translate(bannerPos.x, bannerPos.y, 3);
		modelStack.Scale(100, 100, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Quitting"));
		modelStack.PopMatrix();
	}

	controller.RenderUI();
	EntityManager::GetInstance()->RenderUI();
}

void GameplayScene::Exit()
{
	// Detach camera from other entities
	BGM->stop();
	BGM->drop();
	BGM = nullptr;
	EntityManager::GetInstance()->RemoveEntity(turnDisplay);
	EntityManager::GetInstance()->RemoveEntity(groundEntity);
	//EntityManager::GetInstance()->RemoveEntity(fps);
	turnDisplay = nullptr;
	groundEntity = nullptr;
	//playerInfo->DetachCamera();
	
	spawner.ClearSpawns();
	gameMap.ClearCharacters();
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

void GameplayScene::Pause()
{
	BGM->setIsPaused(true);
	//groundEntity->SetIsDone(true);
	//fps->SetIsDone(true);
	//turnDisplay->SetIsDone(true);
	EntityManager::GetInstance()->RemoveEntity(groundEntity);
	//EntityManager::GetInstance()->RemoveEntity(fps);
	EntityManager::GetInstance()->RemoveEntity(turnDisplay);
	groundEntity = nullptr;
	//fps = nullptr;
	turnDisplay = nullptr;
	b_textRunning = false;
}

void GameplayScene::Resume()
{
	BGM->setIsPaused(false);

	controller.b_cameraTransition = false;
	camera.f_OrthoSize = controller.tempOrtho;

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
	// Customise the ground entity
	groundEntity->SetPosition(Vector3(200, 0, 200));
	groundEntity->SetScale(Vector3(2500.0f, 2500.0f, 2500.0f));
	groundEntity->SetGrids(Vector3(1.0f, 1.0f, 1.0f));

	float fontSize = 5.0f;

	//fps = Create::Text2DObject("text", Vector3(-100, -95, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));

	if (PlayerInfo::GetInstance()->b_attacking)
	{
		if (PlayerInfo::GetInstance()->player)
		{
			PlayerInfo::GetInstance()->player->b_tookAction = true;
			if (PlayerInfo::GetInstance()->enemy->getCurrentHP() <= 0)
			{
				gameMap.RemoveEnemy(PlayerInfo::GetInstance()->enemy);
				PlayerInfo::GetInstance()->enemy = nullptr;
			}
			for (int i = 0; i < gameMap.characters.size(); ++i)
			{
				if (gameMap.characters[i]->b_tookAction == false)
				{
					controller.selectedTile = gameMap.characters[i]->getPos();
					camera.SetCameraPos(Vector3(controller.selectedTile.x * gameMap.tileSizeX + gameMap.tileSizeX / 2, camera.GetCameraPos().y, controller.selectedTile.y * gameMap.tileSizeY + gameMap.tileSizeY / 2));
					camera.SetCameraTarget(Vector3(controller.selectedTile.x * gameMap.tileSizeX + gameMap.tileSizeX / 2, camera.GetCameraTarget().y, controller.selectedTile.y * gameMap.tileSizeY + gameMap.tileSizeY / 2));
					break;
				}
			}
		}		
	}
	else if (!PlayerInfo::GetInstance()->b_attacking)
	{
		if (PlayerInfo::GetInstance()->enemy)
		{
			PlayerInfo::GetInstance()->enemy->b_tookAction = true;
			if (PlayerInfo::GetInstance()->player->getCurrentHP() <= 0)
			{
				gameMap.RemovePlayer(PlayerInfo::GetInstance()->player);
				PlayerInfo::GetInstance()->player = nullptr;
			}
		}
	}
}

void GameplayScene::DisplayText(string text,Vector3 color)
{
	b_textRunning = true;
	turnDisplay = Create::Text2DObject("text", Vector3(-120, 0, 1.0f), text, Vector3(10, 10, 10), Color(color.x, color.y, color.z));
}

void GameplayScene::DisplayWin(bool win)
{
	if (!b_renderWin && !b_renderLose)
	{
		b_renderWin = win;
		b_renderLose = !win;
		bannerPos = Vector2(0, 100);
	}
}