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
#include "AI\AI_DefenceFSM.h"


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

	//BGM = Music::GetInstance()->playSound("Sounds//bossfight.mp3", true, false, true);
	//BGM->setVolume(0.3);

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
	LoadMeshes();

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
	f_shakedelay = 0;
	//f_shakedelayReturn = 0;

	i_shakecounter = 0;

	b_shaking = false;
	shake_dir = false;
	shoot_once = false; //Non-bonus mode, caps and checks for single shot to return
	shoot_hit = false;
	b_isClashed = true;	//on start clash is true (clashed)
	b_bonusRush = PlayerInfo::GetInstance()->b_bonus;
	if (b_bonusRush)
	{
		PlayerInfo::GetInstance()->b_bonus = false;
	}

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

	if (f_shakedelay > 0)
		f_shakedelay -= dt;
	/*if (f_shakedelayReturn > 0)
	f_shakedelayReturn -= dt;*/


	if (f_SceneIntroDelay > 0)
		f_SceneIntroDelay -= dt;
	else
	{
		if (PlayerInfo::GetInstance()->b_attacking)
		{
			if ((!b_bonusRush && !b_spamLock))
			{
				b_isClashed = false;
			}
		}
		else
		{
			if ((!b_bonusRush && !b_spamLock))
			{
				b_isClashed = false;
			}
		}
	}

	if (PlayerInfo::GetInstance()->b_attacking)
	{
		if (PlayerInfo::GetInstance()->player->i_attackRange > 1)
			RunBattleAnimation(dt, true, player->getDamage());
		else
			RunBattleAnimation(dt, false, player->getDamage());
	}
	else
	{
		if (PlayerInfo::GetInstance()->enemy->i_attackRange > 1)
			RunBattleAnimation(dt, true, enemy->getDamage());
		else
			RunBattleAnimation(dt, false, enemy->getDamage());
	}


	for (vector<float>::iterator it = f_projectiles.begin(); it != f_projectiles.end();)
	{
		float *posX = &*it;

		if (*posX < 120)
		{
			(*posX) += 100 * dt;
			it++;
		}
		else
		{
			it = f_projectiles.erase(it);
		}
	}

	for (vector<float>::iterator it = f_projectiles_enemy.begin(); it != f_projectiles_enemy.end();)
	{
		float *posXene = &*it;
		

		if (*posXene > 70)
		{
			

			
				*posXene -= 100 * dt;
			
			it++;
		}
		else
		{
			
			it = f_projectiles_enemy.erase(it);
			shoot_hit = true;
		}
	}

	LightMouseControl(dt);

	//camera.Update(dt);

	TakenHitAnimation(dt);

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
	//if (KeyboardController::GetInstance()->IsKeyDown('1'))
	//	glEnable(GL_CULL_FACE);
	//if (KeyboardController::GetInstance()->IsKeyDown('2'))
	//	glDisable(GL_CULL_FACE);
	//if (KeyboardController::GetInstance()->IsKeyDown('3'))
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//if (KeyboardController::GetInstance()->IsKeyDown('4'))
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//if (KeyboardController::GetInstance()->IsKeyDown('5'))
	//{
	//	lights[0]->type = Light::LIGHT_POINT;
	//}
	//else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	//{
	//	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	//}
	//else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	//{
	//	lights[0]->type = Light::LIGHT_SPOT;
	//}

	if (KeyboardController::GetInstance()->IsKeyReleased('Z'))
	{
		//Feel free to Remove/Edit.

		if (!b_spamLock)
			b_isClashed = false;	//when m = no clash, start clashing

		if (!b_bonus_start && b_bonusRush && PlayerInfo::GetInstance()->b_attacking)
		{
			b_bonus_start = true;
			f_bonus_time = 5;
			TimerText->SetPosition(Vector3(-180, 120, 1));
			TimerText->SetText("TIME:");
		}
	}

	//animation always running
	//RunBattleAnimation(dt, false, 123);


	if (KeyboardController::GetInstance()->IsKeyPressed('C'))
	{
		if (b_bonusRush && b_spamLock)
			SceneManager::GetInstance()->SetActiveScene("GameState");
	}
	//if (KeyboardController::GetInstance()->IsKeyDown('P'))
	//{

	//	f_projectiles_enemy.push_back(110);

	//	//f_shakedelay = 0.3f;
	//	//b_shaking = true;
	//	//i_shakecounter = 5;
	//	//TakenHitAnimation(dt);
	//	//CameraClash(true, dt);
	//}
	/*if (KeyboardController::GetInstance()->IsKeyDown('O'))
	{
	CameraClashReturn(dt);
	}*/


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

void BattleScene::PlayerAnimation(double dt, bool ranged, int dmgvalue, float maxdist)
{
	if (!b_isClashed)
	{
		if (player_posx <= maxdist)		//Move from left to right
		{
			CameraClash(false, ranged, dt);

			if (!ranged)
			{
				if (player_posx >= maxdist - 10)
				{
					if (b_bonusRush)	//Dont slow down on Bonus
						player_posx += dt * 100;
					else
						player_posx += dt * 5;
				}
				else
					player_posx += dt * 100;
			}
			else
			{
				if (player_posx >= maxdist - 5)
				{
					if (b_bonusRush)	//Dont slow down on Bonus
						player_posx += dt * 100;
					else
						player_posx += dt * 5;
				}
				else
					player_posx += dt * 100;
			}

		}
		else	//reached right
		{

			b_isClashed = true;	//clash true
			/*i_shakecounter = 5;
			TakenHitAnimation(dt);*/
			f_shakedelay = 0.2f;
			if (ranged)
			{
				f_projectiles.push_back(85.f);
			}

			if (player->attack(enemy))
			{
				//cout << enemy->getHP() << endl;
				f_textDelayOnScreen = 5;

				DamageText* tempdmg = new DamageText();
				tempdmg->dmgTxt = Create::Text3DObject(("text"), Vector3(enemy_posx - 6, -20, 60), std::to_string(dmgvalue*-1), Vector3(5, 5, 5), Vector3(1, 0, 0), 180.f, Color(1, 0.3, 0.3));
				storeDmgTxt.push_back(tempdmg);
				i_totaldmg_txt += dmgvalue;
			}
			else if (!player->attack(enemy))
			{
				f_textDelayOnScreen = 5;

				DamageText* tempdmg = new DamageText();
				tempdmg->dmgTxt = Create::Text3DObject(("text"), Vector3(enemy_posx - 6, -20, 60), "MISS", Vector3(5, 5, 5), Vector3(1, 0, 0), 180.f, Color(1, 0.3, 0.3));
				storeDmgTxt.push_back(tempdmg);
			}



			TotalDamage->SetText("TOTAL DAMAGE:" + std::to_string(i_totaldmg_txt));
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
		//CameraClashReturn(dt);
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

void BattleScene::EnemyAnimation(double dt, bool ranged, int dmgvalue, float maxdist)
{


	if (!b_isClashed)
	{

		if (enemy_posx >= maxdist)		//Move from right to left
		{
			CameraClash(true, ranged, dt);

			if (enemy_posx <= maxdist + 10)
			{
				if (ranged)
				{
					enemy_posx -= dt * 5;
					shoot_once = true;
				}
				else
					enemy_posx -= dt * 5;
			}
			else
			{
				enemy_posx -= dt * 100;
			}

		}

		else
		{
			b_isClashed = true;
			/*i_shakecounter = 5;
			TakenHitAnimation(dt);*/
			f_shakedelay = 0.2f;

			if (enemy->attack(player))
			{
				f_textDelayOnScreen = 5;

				DamageText* tempdmg = new DamageText();
				tempdmg->dmgTxt = Create::Text3DObject(("text"), Vector3(player_posx - 3, -20, 60), std::to_string(dmgvalue*-1), Vector3(5, 5, 5), Vector3(1, 0, 0), 180.f, Color(1, 0.3f, .3f));
				storeDmgTxt.push_back(tempdmg);
				i_totaldmg_txt += dmgvalue;
			}
			else if (!enemy->attack(player))
			{
				f_textDelayOnScreen = 5;

				DamageText* tempdmg = new DamageText();
				tempdmg->dmgTxt = Create::Text3DObject(("text"), Vector3(player_posx - 3, -20, 60), "MISS", Vector3(5, 5, 5), Vector3(1, 0, 0), 180.f, Color(1, 0.3f, .3f));
				storeDmgTxt.push_back(tempdmg);
			}



			//i_totaldmg_txt += dmgvalue;
			TotalDamage->SetText("TOTAL DAMAGE:" + std::to_string(i_totaldmg_txt));
			TotalDamage->SetScale(Vector3(23, 35, 20));

			//PlayerInfo::GetInstance()->enemy->attack();
			//TakenHitAnimation(player_posx);

			//if (!b_bonusRush)
			b_spamLock = true;

		}
	}
	else	//clash is true
	{
		if (shoot_once)
		{
			f_projectiles_enemy.push_back(110);
			shoot_once = false;
		}

		//CameraClashReturn(dt);
		if (enemy_posx < 120)		//Return to right from left(original position)
		{
			enemy_posx += dt * 50;
		}

		else
		{
			//SceneManager::GetInstance()->SetActiveScene("GameState");
			if (b_spamLock)
			{
				if (ranged)
				{
					if (shoot_hit)
					{
						SceneManager::GetInstance()->SetActiveScene("GameState");
						shoot_hit = false;
					}
				}
				else
					SceneManager::GetInstance()->SetActiveScene("GameState");
			}
			//std::cout << "do something" << std::endl;
		}
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
		PlayerAnimation(dt, ranged, dmgvalue, maxdist_forward);
	}
	else
	{
		EnemyAnimation(dt, ranged, dmgvalue, ene_maxdist_forward);
	}
}

void BattleScene::TakenHitAnimation(double dt)
{
	if (f_shakedelay > 0)
	{
		if (!shake_dir)
		{
			camera.SetCameraPos(Vector3(camera.GetCameraPos().x, camera.GetCameraPos().y - (dt * 100), camera.GetCameraPos().z));
			camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x, camera.GetCameraTarget().y - (dt * 100), camera.GetCameraTarget().z));

			if (camera.GetCameraPos().y <= -22)
			{
				shake_dir = true;
			}
		}

		if (shake_dir)
		{
			camera.SetCameraPos(Vector3(camera.GetCameraPos().x, camera.GetCameraPos().y + (dt * 100), camera.GetCameraPos().z));
			camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x, camera.GetCameraTarget().y + (dt * 100), camera.GetCameraTarget().z));

			if (camera.GetCameraPos().y >= -18)
			{
				shake_dir = false;
			}
		}
	}
	if (f_shakedelay <= 0 && b_isClashed)
	{
		CameraClashReturn(dt);
	}

}

void BattleScene::CameraClash(bool is_player, bool ranged, double dt)
{
	//float temp_x = 100;
	if (is_player)	//Zoom to player
	{
		if (camera.GetCameraTarget().x >= (80 + 3))
		{
			if (!ranged)
			{
				if (camera.GetCameraTarget().x <= (80 + 8))
				{
					camera.SetCameraPos(Vector3(camera.GetCameraTarget().x - (dt * 3), camera.GetCameraPos().y + (dt * 1), camera.GetCameraPos().z + (dt * 6)));
					camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x - (dt * 3), camera.GetCameraTarget().y + (dt * 1), 100));
				}
				else
				{
					camera.SetCameraPos(Vector3(camera.GetCameraTarget().x - (dt * 60), camera.GetCameraPos().y + (dt * 20), camera.GetCameraPos().z + (dt * 120)));
					camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x - (dt * 60), camera.GetCameraTarget().y + (dt * 20), 100));
				}
			}
			else
			{
				if (camera.GetCameraTarget().x <= (80 + 15))
				{
					camera.SetCameraPos(Vector3(camera.GetCameraTarget().x - (dt * 3), camera.GetCameraPos().y + (dt * 1), camera.GetCameraPos().z + (dt * 6)));
					camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x - (dt * 3), camera.GetCameraTarget().y + (dt * 1), 100));
				}
				else
				{
					camera.SetCameraPos(Vector3(camera.GetCameraTarget().x - (dt * 60), camera.GetCameraPos().y + (dt * 20), camera.GetCameraPos().z + (dt * 120)));
					camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x - (dt * 60), camera.GetCameraTarget().y + (dt * 20), 100));
				}
			}
		}
	}
	else		//Zoom to Enemy
	{
		if (camera.GetCameraTarget().x <= (120 - 3))
		{
			if (!ranged)
			{
				if (camera.GetCameraTarget().x >= (120 - 8))
				{
					camera.SetCameraPos(Vector3(camera.GetCameraTarget().x + (dt * 3), camera.GetCameraPos().y + (dt * 1), camera.GetCameraPos().z + (dt * 6)));
					camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x + (dt * 3), camera.GetCameraTarget().y + (dt * 1), 100));
				}
				else
				{
					camera.SetCameraPos(Vector3(camera.GetCameraTarget().x + (dt * 60), camera.GetCameraPos().y + (dt * 20), camera.GetCameraPos().z + (dt * 120)));
					camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x + (dt * 60), camera.GetCameraTarget().y + (dt * 20), 100));
				}
			}
			else	//ranged type slows camera on the start
			{
				if (camera.GetCameraTarget().x >= (120 - 15))
				{
					camera.SetCameraPos(Vector3(camera.GetCameraTarget().x + (dt * 3), camera.GetCameraPos().y + (dt * 1), camera.GetCameraPos().z + (dt * 6)));
					camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x + (dt * 3), camera.GetCameraTarget().y + (dt * 1), 100));
				}
				else
				{
					camera.SetCameraPos(Vector3(camera.GetCameraTarget().x + (dt * 60), camera.GetCameraPos().y + (dt * 20), camera.GetCameraPos().z + (dt * 120)));
					camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x + (dt * 60), camera.GetCameraTarget().y + (dt * 20), 100));
				}
			}
		}
	}
}

void BattleScene::CameraClashReturn(double dt)
{
	//Return from player zoom
	if (camera.GetCameraTarget().x < 100 && camera.GetCameraPos().z > -10)
	{
		camera.SetCameraPos(Vector3(camera.GetCameraTarget().x + (dt * 60), camera.GetCameraPos().y - (dt * 20), camera.GetCameraPos().z - (dt * 130)));
		camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x + (dt * 60), camera.GetCameraTarget().y - (dt * 20), 100));
	}
	//Return from enemy zoom
	else if (camera.GetCameraTarget().x > 100 && camera.GetCameraPos().z > -10)
	{
		camera.SetCameraPos(Vector3(camera.GetCameraTarget().x - (dt * 60), camera.GetCameraPos().y - (dt * 20), camera.GetCameraPos().z - (dt * 130)));
		camera.SetCameraTarget(Vector3(camera.GetCameraTarget().x - (dt * 60), camera.GetCameraTarget().y - (dt * 20), 100));
	}
	else
	{
		camera.SetCameraPos(Vector3(100, -20, -10));
		camera.SetCameraTarget(Vector3(100, -19, 100));
	}
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

	//RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Fps:" + std::to_string(fps), Vector3(-200, 180, 10), 10, Color(1, 1, 1));
	//RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Cam:" + std::to_string(camera.GetCameraPos().x) + "," + std::to_string(camera.GetCameraPos().y) + "," + std::to_string(camera.GetCameraPos().z), Vector3(-200, 160, 10), 10, Color(1, 1, 1));
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	float playerhptemp;
	float enemyhptemp;


	playerhptemp = ((float)(player->getCurrentHP()) / (float)(player->getMaxHP())) * 50.f;
	enemyhptemp = ((float)(enemy->getCurrentHP()) / (float)(enemy->getMaxHP())) * 50.f;

	if (player)
	{

		modelStack.PushMatrix();
		modelStack.Translate(-130, -150, 10);
		modelStack.Scale(playerhptemp, 10, 5);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GREEN_BAR"));
		modelStack.PopMatrix();
	}
	if (enemy)
	{
		modelStack.PushMatrix();
		modelStack.Translate(80, -150, 10);
		modelStack.Scale(enemyhptemp, 10, 5);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GREEN_BAR"));
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(-140, 120, 0);
	modelStack.Scale(f_bonus_time * 50, 10, 10);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("YELLOW_BAR"));
	modelStack.PopMatrix();

}

void BattleScene::RenderSkyBox()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	modelStack.PushMatrix();
	modelStack.Translate(100, -100, 700);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT"));
	modelStack.PopMatrix();


}

void BattleScene::LoadMeshes()
{
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
	MeshBuilder::GetInstance()->GenerateOBJ("ICE_KNIGHT_RED", "OBJ//iceknight_obj.obj");
	MeshBuilder::GetInstance()->GetMesh("ICE_KNIGHT_RED")->textureID = LoadTGA("Image//iceknight_red.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ICE_KNIGHT_YELLOW", "OBJ//iceknight_obj.obj");
	MeshBuilder::GetInstance()->GetMesh("ICE_KNIGHT_YELLOW")->textureID = LoadTGA("Image//iceknight_yellow.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("ICE_KNIGHT_GREEN", "OBJ//iceknight_obj.obj");
	MeshBuilder::GetInstance()->GetMesh("ICE_KNIGHT_GREEN")->textureID = LoadTGA("Image//iceknight_green.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("GREEN_BAR", "OBJ//bar_bar.obj");
	MeshBuilder::GetInstance()->GetMesh("GREEN_BAR")->textureID = LoadTGA("Image//greenTexture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("YELLOW_BAR", "OBJ//bar_bar.obj");
	MeshBuilder::GetInstance()->GetMesh("YELLOW_BAR")->textureID = LoadTGA("Image//yellowTexture.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("BASIC_TREE", "OBJ//Tree_low.obj");
	MeshBuilder::GetInstance()->GetMesh("BASIC_TREE")->textureID = LoadTGA("Image//Tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("BASIC_PLANT", "OBJ//Plant_low.obj");
	MeshBuilder::GetInstance()->GetMesh("BASIC_PLANT")->textureID = LoadTGA("Image//grass_obj.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("PROJECTILE", "OBJ//Arrow.obj");
	MeshBuilder::GetInstance()->GetMesh("PROJECTILE")->textureID = LoadTGA("Image//grass_lightgreen.tga");

}

void BattleScene::RenderProps()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	RenderUnitsModels(modelStack);

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

void BattleScene::RenderUnitsModels(MS& _ms)
{
	if (enemy->strategy == Character::DEFENCE)
	{
		//Right - Enemy Render
		_ms.PushMatrix();
		_ms.Translate(enemy_posx, 0, 60);
		_ms.Scale(3, 3, 3);
		_ms.Rotate(180, 0, 0, 1);
		_ms.Rotate(110, 0, 1, 0);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("ICE_KNIGHT_GREEN"));
		_ms.PopMatrix();
	}
	else
	{
		_ms.PushMatrix();
		_ms.Translate(enemy_posx, 0, 60);
		_ms.Scale(3, 3, 3);
		_ms.Rotate(180, 0, 0, 1);
		_ms.Rotate(110, 0, 1, 0);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("ICE_KNIGHT_RED"));
		_ms.PopMatrix();
	}
	//Left - Player Render
	_ms.PushMatrix();
	_ms.Translate(player_posx, 0, 60);
	_ms.Scale(3, 3, 3);
	_ms.Rotate(180, 0, 0, 1);
	_ms.Rotate(-110, 0, 1, 0);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("ICE_KNIGHT"));
	_ms.PopMatrix();



	for (int i = 0; i < f_projectiles.size(); ++i)
	{
		_ms.PushMatrix();
		_ms.Translate(f_projectiles[i], -10, 100);
		_ms.Scale(50, 25, 50);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("PROJECTILE"));
		_ms.PopMatrix();
	}
	for (int i = 0; i < f_projectiles_enemy.size(); ++i)
	{
		_ms.PushMatrix();
		_ms.Translate(f_projectiles_enemy[i], -10, 100);
		_ms.Scale(50, 25, 50);
		_ms.Rotate(180, 0, 1, 0);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("PROJECTILE"));
		_ms.PopMatrix();
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


	if (!b_bonus_start && b_bonusRush && PlayerInfo::GetInstance()->b_attacking)
	{

		TotalDmgCheer->SetPosition(Vector3(-100, 130, 0));
		TotalDmgCheer->SetScale(Vector3(30, 30, 30));
		TotalDmgCheer->SetColor(Color(1, 1, 0));

		TotalDmgCheer->SetText("BONUS ATTACK");


		TimerText->SetPosition(Vector3(-80, 100, 0));
		TimerText->SetText("(Mash Z to Begin!)");

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
			TimerText->SetText("(Press C to return...)");
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


	if (i_totaldmg_txt >= (dmgvalue * 10) && f_textDelayOnScreen > 0)
	{
		if (dmgvalue > 0)
		{
			TotalDamage->SetColor(Color(1, 0, 0));
			TotalDmgCheer->SetColor(Color(1, 0, 0));
			TotalDmgCheer->SetScale(TotalDamage->GetScale());
			TotalDmgCheer->SetText("GOOD");
			if (i_totaldmg_txt >= (dmgvalue * 20))
			{

				TotalDamage->SetColor(Color(1, 1, 0));
				TotalDmgCheer->SetColor(Color(1, 1, 0));
				TotalDmgCheer->SetScale(TotalDamage->GetScale());
				TotalDmgCheer->SetText("EXCELLENT");
			}
		}
	}


	//modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - (-1615), camera.position.z - 125)), 0, 1, 0);
}

void BattleScene::Exit()
{
	// Detach camera from other entities
	//BGM->stop();
	//BGM->drop();
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RemoveEntity(groundEntity);
	EntityManager::GetInstance()->RemoveEntity(TotalDmgCheer);
	EntityManager::GetInstance()->RemoveEntity(TotalDamage);
	EntityManager::GetInstance()->RemoveEntity(TimerText);
	//groundEntity->SetIsDone(true);
	//TotalDmgCheer->SetIsDone(true);
	//TotalDamage->SetIsDone(true);
	enemy = nullptr;
	player = nullptr;

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