#include "BattleScene.h"
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
#include "MyMath.h"


#include <iostream>
using namespace std;


BattleScene::BattleScene()
{
}


BattleScene::~BattleScene()
{
	
}

void BattleScene::Init()
{
	currProg = GraphicsManager::GetInstance()->GetActiveShader();

	BGM = Music::GetInstance()->playSound("Sounds//bossfight.mp3", true, false, true);
	BGM->setVolume(0.3);

	//lights[0] = new Light();
	//GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	//lights[0]->type = Light::LIGHT_DIRECTIONAL;
	//lights[0]->position.Set(0, -10, 0);
	//lights[0]->color.Set(1, 1, 0.7);
	//lights[0]->power = 1;
	//lights[0]->kC = 1.f;
	//lights[0]->kL = 0.01f;
	//lights[0]->kQ = 0.001f;
	//lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	//lights[0]->exponent = 3.f;
	//lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	//lights[0]->name = "lights[0]";

	//lights[1] = new Light();
	//GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	//lights[1]->type = Light::LIGHT_DIRECTIONAL;
	//lights[1]->position.Set(1, 1, 0);
	//lights[1]->color.Set(1, 1, 0.5f);
	//lights[1]->power = 0.4f;
	//lights[1]->name = "lights[1]";

	//currProg->UpdateInt("numLights", 1);
	//currProg->UpdateInt("textEnabled", 0);

	// Create and attach the camera to the scene
	camera.Init(Vector3(100, -20, -10), Vector3(100, -19, 100), Vector3(0, 0, 1));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");

	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//grisaiaCustom.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);

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

	MeshBuilder::GetInstance()->GenerateOBJ("ICE_KNIGHT", "OBJ//iceknight_obj.obj");
	MeshBuilder::GetInstance()->GetMesh("ICE_KNIGHT")->textureID = LoadTGA("Image//iceknight_texture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BAR_BAR", "OBJ//bar_bar.obj");

	MeshBuilder::GetInstance()->GenerateOBJ("BASIC_TREE", "OBJ//Tree_low.obj");
	MeshBuilder::GetInstance()->GetMesh("BASIC_TREE")->textureID = LoadTGA("Image//Tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BASIC_PLANT", "OBJ//Plant_low.obj");
	MeshBuilder::GetInstance()->GetMesh("BASIC_PLANT")->textureID = LoadTGA("Image//grass_obj.tga");

	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(100, 0, 100));
	groundEntity->SetScale(Vector3(750.0f, 750.0f, 750.0f));
	groundEntity->SetGrids(Vector3(1.0f, 1.0f, 1.0f));
	//playerInfo->SetTerrain(groundEntity);

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
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
	
	//Init Position of models;
	player_posx = 80;
	enemy_posx = 120;
	f_textDelayOnScreen = 0;
	f_SceneIntroDelay = 1;

	b_isClashed = true;	//on start clash is true (clashed)
	b_bonusRush = true; //Set if bonus mode is true/false	//IMPORTANT : Decides performance of the BattleScene

	b_spamLock = false; //Should not be touched. Automatically locks if not bonus + clashed once

	f_bonus_time = 0;	//Timer for bonus duration
	b_bonus_start = false; //Bool to start bonus mode countdown

	i_totaldmg_txt = 0;
	TotalDamage = Create::Text2DObject(("text"), Vector3(-180, 160, 1), " ", Vector3(20, 20, 20), Color(0, 0, 0));
	TotalDmgCheer = Create::Text2DObject(("text"), Vector3(-180, 140, 1), " ", Vector3(20, 20, 20), Color(0, 0, 0));
	TimerText = Create::Text2DObject(("text"), Vector3(-180, 120, 1), " ", Vector3(15, 15, 15), Color(0.7, 0.7, 1));
	

	fps = 0.f;
	
	/*player = new MeleeCharacter();
	Weapon* wtf = new Weapon(100, 50, false);
	player->equipWeapon(wtf);*/

	player = PlayerInfo::GetInstance()->player;
	enemy = PlayerInfo::GetInstance()->enemy;
}

void BattleScene::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	
	fps = 1 / dt;
	
	if (f_SceneIntroDelay > 0)
		f_SceneIntroDelay -= dt;
	else
	{
		if (!b_bonusRush && !b_spamLock)
		{
			b_isClashed = false;
		}
	}

	//animation always running
	if (player)
		RunBattleAnimation(dt, false, player->getDamage());

	LightMouseControl(dt);
	

	//camera.Update(dt);


	
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	//playerInfo->Update(dt);

	//Music::GetInstance()->SetListener(playerInfo->GetPos(), playerInfo->GetPos() - playerInfo->GetTarget());

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	//std::ostringstream ss;
	//ss.precision(5);
	//float fps = (float)(1.f / dt);
	//ss << "FPS: " << fps;
	//textObj[1]->SetText(ss.str());

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

void BattleScene::LightMouseControl(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		//Feel free to Remove/Edit.

		if (!b_spamLock)
			b_isClashed = false;	//when m = no clash, start clashing

		if (!b_bonus_start && b_bonusRush)
		{
			b_bonus_start = true;
			f_bonus_time = 5;
			TimerText->SetPosition(Vector3(-180, 120, 1));
			TimerText->SetText("TIME:");
		}
	}

	//animation always running
	//RunBattleAnimation(dt, false, 123);


	if (KeyboardController::GetInstance()->IsKeyPressed('K'))
	{
		if (b_bonusRush && b_spamLock)
			SceneManager::GetInstance()->SetActiveScene("GameState");
	}
	//camera.Update(dt);


	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		//cout << "Left Mouse Button was released!" << endl;
		//		player->attack();
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

}

void BattleScene::RunBattleAnimation(double dt, bool ranged, int dmgvalue)
{

	float maxdist_forward = 0;
	float ene_maxdist_forward = 0;

	if (ranged)
	{
		maxdist_forward = 90;
		ene_maxdist_forward = 110;
	}
	else
	{
		maxdist_forward = 115;
		ene_maxdist_forward = 85;
	}

	//if (dmgvalue == NULL)
	//{
	//	dmgvalue = 0;
	//}
	
	RenderTextStuff(dt, dmgvalue);

	//if player attacking
	if (PlayerInfo::GetInstance()->b_attacking)
	{
		if (!b_isClashed)
		{
			if (player_posx <= maxdist_forward)		//Move from left to right
			{
				player_posx += dt * 50;
			}
			else	//reached right
			{

				b_isClashed = true;	//clash true

				player->attack(enemy);

				//cout << enemy->getHP() << endl;
				f_textDelayOnScreen = 5;

				DamageText* tempdmg = new DamageText();
				tempdmg->dmgTxt = Create::Text3DObject(("text"), Vector3(enemy_posx-6, -20, 60), std::to_string(dmgvalue*-1), Vector3(5, 5, 5), Vector3(1, 0, 0), 180.f, Color(1, 0.3, 0.3));
				storeDmgTxt.push_back(tempdmg);
				

				i_totaldmg_txt += dmgvalue;
				TotalDamage->SetText("TOTAL DAMAGE:"+std::to_string(i_totaldmg_txt));
				TotalDamage->SetScale(Vector3(23, 35, 20));

				if (!b_bonusRush)
					b_spamLock = true;
				//PlayerInfo::GetInstance()->player->attack();
				//TakenHitAnimation(enemy_posx);
			}
		}
		else
		{
			//if clash is true

			if (player_posx > 80)		//Return to left from right(original position)
			{
				player_posx -= dt * 50;
			}
			else
			{
				if (!b_bonusRush && b_spamLock && f_textDelayOnScreen <= 3)
				{
					SceneManager::GetInstance()->SetActiveScene("GameState");
				}
			}
		}
	}
	else
	{
		if (!b_isClashed)
		{

			if (enemy_posx >= ene_maxdist_forward)		//Move from right to left
			{
				enemy_posx -= dt * 50;
			}
			else
			{
				b_isClashed = true;

				enemy->attack(player);
				f_textDelayOnScreen = 5;

				DamageText* tempdmg = new DamageText();
				tempdmg->dmgTxt = Create::Text3DObject(("text"), Vector3(player_posx - 3, -20, 60), std::to_string(dmgvalue*-1), Vector3(5, 5, 5), Vector3(1, 0, 0), 180.f, Color(1, 0.3f, .3f));
				storeDmgTxt.push_back(tempdmg);

				i_totaldmg_txt += dmgvalue;
				TotalDamage->SetText("TOTAL DAMAGE:" + std::to_string(i_totaldmg_txt));
				TotalDamage->SetScale(Vector3(23, 35, 20));

				//PlayerInfo::GetInstance()->enemy->attack();
				//TakenHitAnimation(player_posx);

				if (!b_bonusRush)
					b_spamLock = true;
				
			}
		}
		else
		{
			
			if (enemy_posx < 120)		//Return to right from left(original position)
			{
				enemy_posx += dt * 50;
			}
			
			else
			{
				if (!b_bonusRush && b_spamLock)
				{
					SceneManager::GetInstance()->SetActiveScene("GameState");
				}
			}
		}
	}
}

void BattleScene::TakenHitAnimation(float& type_pos)
{

	int pos = 1;
	int neg = -1;
	int neg_diff = type_pos - 10;
	int pos_diff = type_pos + 10;

	int shakecounter = 5;


	/*if (shakecounter > 0)
	{
		if (shakecounter % 2 == 0)
		{
			if (type_pos )
			type_pos += 
		}
		else
		{

		}
	}*/	
	
}

void BattleScene::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//GraphicsManager::GetInstance()->SetOrthographicProjection(-100, 100, -100, 100, -2000, 2000);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	EntityManager::GetInstance()->Render();

	RenderSkyBox();
	RenderProps();
	
	

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;

	GraphicsManager::GetInstance()->SetOrthographicProjection(-200, 200, -200, 200, -2000, 2000);
	GraphicsManager::GetInstance()->DetachCamera();


	EntityManager::GetInstance()->RenderUI();

	RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Fps:" + std::to_string(fps), Vector3(-200, 180, 10),  10, Color(1, 1, 1));

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(-130, -150, 10);
	modelStack.Scale(player->getHP()*0.5, 10, 5);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("BAR_BAR"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(80, -150, 10);
	modelStack.Scale(enemy->getHP()*0.5, 10, 5);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("BAR_BAR"));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-140, 120, 0);
	modelStack.Scale(f_bonus_time*50, 10, 10);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("BAR_BAR"));
	modelStack.PopMatrix();

}

void BattleScene::RenderSkyBox()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	modelStack.PushMatrix();
	modelStack.Translate(100,-100,700);
	modelStack.Scale(1000,1000,1000);
	modelStack.Rotate(180,0,1,0);
	modelStack.Rotate(180,0, 0, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT"));
	modelStack.PopMatrix();


}

void BattleScene::RenderProps()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();


	//Right - Enemy Render
	modelStack.PushMatrix();
	modelStack.Translate(enemy_posx, 0, 60);
	modelStack.Scale(3, 3, 3);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Rotate(110, 0, 1, 0);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("ICE_KNIGHT"));
	modelStack.PopMatrix();

	//Left - Player Render
	modelStack.PushMatrix();
	modelStack.Translate(player_posx, 0, 60);
	modelStack.Scale(3, 3, 3);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Rotate(-110, 0, 1, 0);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("ICE_KNIGHT"));
	modelStack.PopMatrix();



	modelStack.PushMatrix();
	modelStack.Translate(100, 0, 100);
	modelStack.Scale(3, 3, 3);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Rotate(-110, 0, 1, 0);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("BASIC_PLANT"));
	modelStack.PopMatrix();

	for (int i = 40; i <= 160; i += 60)
	{
		modelStack.PushMatrix();
		modelStack.Translate(i, 0, 180);
		modelStack.Scale(8, 8, 8);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Rotate(90, 0, 1, 0);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("BASIC_TREE"));
		modelStack.PopMatrix();
	}

	for (int i = 20; i < 180; i += 30)
	{
		modelStack.PushMatrix();
		modelStack.Translate(i, 0, 150);
		modelStack.Scale(5, 5, 5);
		modelStack.Rotate(180, 0, 0, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("BASIC_TREE"));
		modelStack.PopMatrix();
	}
}

void BattleScene::RenderTextStuff(double dt, int dmgvalue)
{
	//run all damageTexts entities list
	for (vector<DamageText*>::iterator it = storeDmgTxt.begin(); it != storeDmgTxt.end();)
	{
		//Update is bool, return true = max height reached
		if ((*it)->Update(dt))
		{
			delete *it;
			it = storeDmgTxt.erase(it);
		}
		else
		{
			// only continue to next damageEntity when max height reached
			it++;
		}
	}


	if (!b_bonus_start && b_bonusRush)
	{
		
		TotalDmgCheer->SetPosition(Vector3(-100, 130, 0));
		TotalDmgCheer->SetScale(Vector3(30, 30, 30));
		TotalDmgCheer->SetColor(Color(1, 1, 0));

		TotalDmgCheer->SetText("BONUS ATTACK");


		TimerText->SetPosition(Vector3(-80, 100, 0));
		TimerText->SetText("(Mash M to Begin!)");
		
	}
	else
	{
		
	}

	if (f_bonus_time > 0)
	{
		f_bonus_time -= dt * 1;
	}
	else
	{
		if (b_bonus_start)
		{
			b_spamLock = true;

			TotalDamage->SetColor(Color(1, 1, 0));
			TotalDmgCheer->SetColor(Color(1, 1, 0));
			TotalDmgCheer->SetScale(TotalDamage->GetScale());
			TotalDmgCheer->SetText("TIME OVER!");
			TimerText->SetText("(Press K to return...)");
		}
	}

	

	if (TotalDamage->GetScale().y > 20)
	{
		TotalDamage->SetScale(Vector3(TotalDamage->GetScale().x, TotalDamage->GetScale().y, TotalDamage->GetScale().z) - Vector3(0, dt * 50, 0));
	}
	if (TotalDamage->GetScale().x > 20)
	{
		TotalDamage->SetScale(Vector3(TotalDamage->GetScale().x, TotalDamage->GetScale().y, TotalDamage->GetScale().z) - Vector3(dt * 50, 0, 0));
	}

	if (TotalDmgCheer->GetScale().y > 20)
	{
		TotalDmgCheer->SetScale(Vector3(TotalDmgCheer->GetScale().x, TotalDmgCheer->GetScale().y, TotalDmgCheer->GetScale().z) - Vector3(0, dt * 50, 0));
	}
	if (TotalDmgCheer->GetScale().x > 20)
	{
		TotalDmgCheer->SetScale(Vector3(TotalDmgCheer->GetScale().x, TotalDmgCheer->GetScale().y, TotalDmgCheer->GetScale().z) - Vector3(dt * 50, 0, 0));
	}

	if (f_textDelayOnScreen > 0)
	{
		f_textDelayOnScreen -= dt * 1;
	}
	else
	{
		if (b_bonus_start && (f_bonus_time >= 0))
		{
			TotalDamage->SetScale(Vector3(TotalDamage->GetScale().x, TotalDamage->GetScale().y, TotalDamage->GetScale().z) - Vector3(0, dt * 50, 0));
			TotalDamage->SetColor(Color(0, 0, 0));
			i_totaldmg_txt = 0;
			TotalDmgCheer->SetScale(Vector3(TotalDmgCheer->GetScale().x, TotalDmgCheer->GetScale().y, TotalDmgCheer->GetScale().z) - Vector3(0, dt * 50, 0));
			TotalDmgCheer->SetPosition(Vector3(-180, 140, 1));
			TotalDmgCheer->SetText(" ");
			
		}

	}
	

	if (i_totaldmg_txt >= (dmgvalue*50))
	{

		TotalDamage->SetColor(Color(1, 0, 0));
		TotalDmgCheer->SetColor(Color(1, 0, 0));
		TotalDmgCheer->SetScale(TotalDamage->GetScale());
		TotalDmgCheer->SetText("GOOD");
		if (i_totaldmg_txt >= (dmgvalue*100))
		{

			TotalDamage->SetColor(Color(1, 1, 0));
			TotalDmgCheer->SetColor(Color(1, 1, 0));
			TotalDmgCheer->SetScale(TotalDamage->GetScale());
			TotalDmgCheer->SetText("EXCELLENT");
		}
	}


	//modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - (-1615), camera.position.z - 125)), 0, 1, 0);
}

void BattleScene::Exit()
{
	// Detach camera from other entities
	BGM->stop();
	BGM->drop();
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RemoveEntity(groundEntity);
	EntityManager::GetInstance()->RemoveEntity(TotalDmgCheer);
	EntityManager::GetInstance()->RemoveEntity(TotalDamage);
	EntityManager::GetInstance()->RemoveEntity(TimerText);
	//groundEntity->SetIsDone(true);
	//TotalDmgCheer->SetIsDone(true);
	//TotalDamage->SetIsDone(true);

	for (vector<DamageText*>::iterator it = storeDmgTxt.begin(); it != storeDmgTxt.end();)
	{
		EntityManager::GetInstance()->RemoveEntity((*it)->dmgTxt);
		delete *it;
		it = storeDmgTxt.erase(it);
	}

	//playerInfo->DetachCamera();

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
