#include "PartySelectScene.h"
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


PartySelectScene::PartySelectScene()
{
}


PartySelectScene::~PartySelectScene()
{
}

void PartySelectScene::Init()
{
	Math::InitRNG();
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	Music::GetInstance()->Init();

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

	// Create and attach the camera to the scene
	camera.Init(Vector3(100, -10, 100), Vector3(100, 0, 100), Vector3(0, 0, 1));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("PartySelectBG", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("PartySelectBG")->textureID = LoadTGA("Image//PartySelectBackground.tga");
	MeshBuilder::GetInstance()->GenerateQuad("test", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("test")->textureID = LoadTGA("Image//test.tga");
	MeshBuilder::GetInstance()->GenerateQuad("selected", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("selected")->textureID = LoadTGA("Image//selectarrow.tga");
	MeshBuilder::GetInstance()->GenerateQuad("statusScreen", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("statusScreen")->textureID = LoadTGA("Image//statusScreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("rightFacingArrow", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("rightFacingArrow")->textureID = LoadTGA("Image//rightFacingArrow.tga");
	MeshBuilder::GetInstance()->GenerateQuad("selectOption", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("selectOption")->textureID = LoadTGA("Image//selectOption.tga");
	MeshBuilder::GetInstance()->GenerateQuad("changeWhoseEQ", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("changeWhoseEQ")->textureID = LoadTGA("Image//changeWhoseEQ.tga");

	groundEntity = Create::Ground("PartySelectBG", "GEO_GRASS_LIGHTGREEN");
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

	
	Character* kek = new MeleeCharacter("KEK");
	kek->setDamage(696969);
	kek->setPortrait(MeshBuilder::GetInstance()->GetMesh("test"));
	kek->equipWeapon(new Weapon(6969, 100, false, "swordz"));
	kek->equipArmor(new Armor(6, 9, 6, 9, false, "armorz"));
	PlayerInfo::GetInstance()->addCharacter(Vector2(0, 0), kek);
	

	Character* derp = new MeleeCharacter("DERP69KEK");
	derp->setDamage(696969);
	derp->setPortrait(MeshBuilder::GetInstance()->GetMesh("selected"));
	derp->equipWeapon(new Weapon(6969, 100, false, "69 swordz"));
	derp->equipArmor(new Armor(6, 9, 6, 9, false, "69 armorz"));
	PlayerInfo::GetInstance()->addCharacter(Vector2(0, 0), derp);

	Character* d = new MeleeCharacter("d");
	d->setDamage(696969);
	d->setPortrait(MeshBuilder::GetInstance()->GetMesh("rightFacingArrow"));
	d->equipWeapon(new Weapon(6969, 100, false, "69 swordz"));
	d->equipArmor(new Armor(6, 9, 6, 9, false, "69 armorz"));
	PlayerInfo::GetInstance()->addCharacter(Vector2(0, 0), d);

	Character* dik = new MeleeCharacter("deek");
	dik->setDamage(696969);
	dik->setPortrait(MeshBuilder::GetInstance()->GetMesh("test"));
	dik->equipWeapon(new Weapon(6969, 100, false, "69 swordz"));
	dik->equipArmor(new Armor(6, 9, 6, 9, false, "69 armorz"));
	PlayerInfo::GetInstance()->addCharacter(Vector2(0, 0), dik);

	Character* a = new MeleeCharacter("derpina");
	a->setDamage(696969);
	a->setPortrait(MeshBuilder::GetInstance()->GetMesh("selected"));
	a->equipWeapon(new Weapon(6969, 100, false, "69 swordz"));
	a->equipArmor(new Armor(6, 9, 6, 9, false, "69 armorz"));
	PlayerInfo::GetInstance()->addCharacter(Vector2(0, 0), a);

	for (int i = 0; i < 10; i++)
	{
		Character* a = new MeleeCharacter("derpina");
		a->setDamage(696969);
		a->setPortrait(MeshBuilder::GetInstance()->GetMesh("test"));
		a->equipWeapon(new Weapon(6969, 100, false, "69 swordz"));
		a->equipArmor(new Armor(6, 9, 6, 9, false, "69 armorz"));
		PlayerInfo::GetInstance()->addCharacter(Vector2(0, 0), a);
	}

	selectedPos.Set(-47, 52.5f, 0);
	currentScreen = CURR_SCREEN_SELECT_OPTION;
	i_selectedOptionCounter = 0;

	i_selectedUnitsCounter = 0;
	b_statusCursor = 0;
	statusCursorPos.Set(-10, -40, 0);
}

void PartySelectScene::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
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

	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('Z'))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			if (i_selectedOptionCounter == 0) // Change party
			{
				currentScreen = CURR_SCREEN_SELECT_UNITS;
				selectedPos.Set(-60, -10, 0);
				i_selectedUnitsCounter = 0;
			}
			else if (i_selectedOptionCounter == 1) // Change equipment
			{
				currentScreen = CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER;
				selectedPos.Set(-60, 40, 0);
				i_selectedUnitsCounter = 0;
			}
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			if (i_selectedUnitsCounter <= PlayerInfo::GetInstance()->availableUnits.size() - 1)
				currentScreen = CURR_SCREEN_VIEW_STATUS;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			if (b_statusCursor == 0)
			{
				if (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->b_inParty == false)
				{
					for (int i = 0; i < 4; i++)
					{
						if (PlayerInfo::GetInstance()->addCharacterToParty(Vector2(0, 0), PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter), i))
							break;
					}
				}
				else
					PlayerInfo::GetInstance()->removeCharacterFromParty(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->i_idInParty);
			}
			else
				currentScreen = CURR_SCREEN_SELECT_UNITS;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			if (i_selectedUnitsCounter <= PlayerInfo::GetInstance()->availableUnits.size() - 1)
				currentScreen = CURR_SCREEN_VIEW_STATUS;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('X'))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			// go back to level select
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			// return to pre-battle preparations
			currentScreen = CURR_SCREEN_SELECT_OPTION;
			selectedPos.Set(-47, 52.5f, 0);
			i_selectedOptionCounter = 0;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			// return to party select
			currentScreen = CURR_SCREEN_SELECT_UNITS;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			// return to pre-battle preparations
			currentScreen = CURR_SCREEN_SELECT_OPTION;
			selectedPos.Set(-47, 52.5f, 0);
			i_selectedOptionCounter = 0;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RIGHT))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			selectedPos.x += 94;
			if (selectedPos.x > 47)
				selectedPos.x = -47;

			i_selectedOptionCounter += 1;
			if (i_selectedOptionCounter == 2)
				i_selectedOptionCounter = 0;
			else if (i_selectedOptionCounter == 4)
				i_selectedOptionCounter = 2;
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			selectedPos.x += 30;
			if (selectedPos.x > 60)
				selectedPos.x = -60;

			i_selectedUnitsCounter += 1;
			if (i_selectedUnitsCounter == 5)
				i_selectedUnitsCounter = 0;
			else if (i_selectedUnitsCounter == 10)
				i_selectedUnitsCounter = 5;
			else if (i_selectedUnitsCounter == 15)
				i_selectedUnitsCounter = 10;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			i_selectedUnitsCounter+=1;
			if (i_selectedUnitsCounter >= PlayerInfo::GetInstance()->availableUnits.size())
				i_selectedUnitsCounter = 0;
			selectedPos.x += 30;
			if (selectedPos.x > 60)
				selectedPos.x = -60;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			selectedPos.x += 30;
			if (selectedPos.x > 60)
				selectedPos.x = -60;

			i_selectedUnitsCounter += 1;
			if (i_selectedUnitsCounter == 5)
				i_selectedUnitsCounter = 0;
			else if (i_selectedUnitsCounter == 10)
				i_selectedUnitsCounter = 5;
			else if (i_selectedUnitsCounter == 15)
				i_selectedUnitsCounter = 10;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_LEFT))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			selectedPos.x -= 94;
			if (selectedPos.x < -47)
				selectedPos.x = 47;

			i_selectedOptionCounter -= 1;
			if (i_selectedOptionCounter == -1)
				i_selectedOptionCounter = 1;
			else if (i_selectedOptionCounter == 1)
				i_selectedOptionCounter = 3;
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			selectedPos.x -= 30;
			if (selectedPos.x < -60)
				selectedPos.x = 60;

			i_selectedUnitsCounter -= 1;
			if (i_selectedUnitsCounter == -1)
				i_selectedUnitsCounter = 4;
			else if (i_selectedUnitsCounter == 4)
				i_selectedUnitsCounter = 9;
			else if (i_selectedUnitsCounter == 9)
				i_selectedUnitsCounter = 14;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			i_selectedUnitsCounter -= 1;
			if (i_selectedUnitsCounter < 0)
				i_selectedUnitsCounter = PlayerInfo::GetInstance()->availableUnits.size() - 1;

			selectedPos.x -= 30;
			if (selectedPos.x < -60)
				selectedPos.x = 60;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			selectedPos.x -= 30;
			if (selectedPos.x < -60)
				selectedPos.x = 60;

			i_selectedUnitsCounter -= 1;
			if (i_selectedUnitsCounter == -1)
				i_selectedUnitsCounter = 4;
			else if (i_selectedUnitsCounter == 4)
				i_selectedUnitsCounter = 9;
			else if (i_selectedUnitsCounter == 9)
				i_selectedUnitsCounter = 14;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_DOWN))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			selectedPos.y -= 74.5f;
			if (selectedPos.y < -22.f)
				selectedPos.y = 52.5f;

			i_selectedOptionCounter += 2;
			if (i_selectedOptionCounter == 4)
				i_selectedOptionCounter = 0;
			else if (i_selectedOptionCounter == 5)
				i_selectedOptionCounter = 1;
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			selectedPos.y -= 30;
			if (selectedPos.y < -70)
				selectedPos.y = -10;

			i_selectedUnitsCounter += 5;
			if (i_selectedUnitsCounter == 15)
				i_selectedUnitsCounter = 0;
			else if (i_selectedUnitsCounter == 16)
				i_selectedUnitsCounter = 1;
			else if (i_selectedUnitsCounter == 17)
				i_selectedUnitsCounter = 2;
			else if (i_selectedUnitsCounter == 18)
				i_selectedUnitsCounter = 3;
			else if (i_selectedUnitsCounter == 19)
				i_selectedUnitsCounter = 4;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			if (b_statusCursor == 0)
				b_statusCursor = 1;
			else
				b_statusCursor = 0;

			statusCursorPos.y -= 30;
			if (statusCursorPos.y < -70)
				statusCursorPos.y = -40;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			selectedPos.y -= 40;
			if (selectedPos.y < -40)
				selectedPos.y = 40;

			i_selectedUnitsCounter += 5;
			if (i_selectedUnitsCounter == 15)
				i_selectedUnitsCounter = 0;
			else if (i_selectedUnitsCounter == 16)
				i_selectedUnitsCounter = 1;
			else if (i_selectedUnitsCounter == 17)
				i_selectedUnitsCounter = 2;
			else if (i_selectedUnitsCounter == 18)
				i_selectedUnitsCounter = 3;
			else if (i_selectedUnitsCounter == 19)
				i_selectedUnitsCounter = 4;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_UP))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			selectedPos.y += 74.5f;
			if (selectedPos.y > 52.5f)
				selectedPos.y = -22.f;

			i_selectedOptionCounter -= 2;
			if (i_selectedOptionCounter == -2)
				i_selectedOptionCounter = 2;
			else if (i_selectedOptionCounter == -1)
				i_selectedOptionCounter = 3;
		}
		if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			selectedPos.y += 30;
			if (selectedPos.y > -10)
				selectedPos.y = -70;

			i_selectedUnitsCounter -= 5;
			if (i_selectedUnitsCounter == -5)
				i_selectedUnitsCounter = 10;
			else if (i_selectedUnitsCounter == -4)
				i_selectedUnitsCounter = 11;
			else if (i_selectedUnitsCounter == -3)
				i_selectedUnitsCounter = 12;
			else if (i_selectedUnitsCounter == -2)
				i_selectedUnitsCounter = 13;
			else if (i_selectedUnitsCounter == -1)
				i_selectedUnitsCounter = 14;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			if (b_statusCursor == 0)
				b_statusCursor = 1;
			else
				b_statusCursor = 0;

			statusCursorPos.y += 30;
			if (statusCursorPos.y > -40)
				statusCursorPos.y = -70;
		}
		if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			selectedPos.y += 40;
			if (selectedPos.y > 40)
				selectedPos.y = -40;

			i_selectedUnitsCounter -= 5;
			if (i_selectedUnitsCounter == -5)
				i_selectedUnitsCounter = 10;
			else if (i_selectedUnitsCounter == -4)
				i_selectedUnitsCounter = 11;
			else if (i_selectedUnitsCounter == -3)
				i_selectedUnitsCounter = 12;
			else if (i_selectedUnitsCounter == -2)
				i_selectedUnitsCounter = 13;
			else if (i_selectedUnitsCounter == -1)
				i_selectedUnitsCounter = 14;
		}
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

void PartySelectScene::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Setup 3D pipeline then render 3D
	//GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->SetOrthographicProjection(-100, 100, -100, 100, -2000, 2000);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	EntityManager::GetInstance()->Render();

	
	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;

	//GraphicsManager::GetInstance()->SetOrthographicProjection(-200, 200, -200, 200, -2000, 2000);
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();

	if (currentScreen == CURR_SCREEN_SELECT_OPTION)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -1);
		modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selectOption"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(selectedPos.x, selectedPos.y, 1);
		modelStack.Scale(87, 40 * 16 / 9, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selected"));
		modelStack.PopMatrix();
	}
	else if (currentScreen == CURR_SCREEN_SELECT_UNITS) // In the select units screen
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -1);
		modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("PartySelectBG"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(selectedPos.x, selectedPos.y, 1);
		modelStack.Scale(15, 15 * 16 / 9, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selected"));
		modelStack.PopMatrix();

		for (int y = 0, counter = 0; y >= -3; y--)
		{
			for (int x = 0; x < 5; x++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(-60 + (x * 30), (y * 30) - 10, 0);
				modelStack.Scale(15, 15 * 16 / 9, 1);
				RenderHelper::RenderMesh(PlayerInfo::GetInstance()->availableUnits.at(counter)->getPortrait());
				modelStack.PopMatrix();
				counter++;
				if (counter >= PlayerInfo::GetInstance()->availableUnits.size())
					break;
			}
			if (counter >= PlayerInfo::GetInstance()->availableUnits.size())
				break;
		}

		
		for (int i = 0,counter = 0; i < 5; i++)
		{
			if (counter >= PlayerInfo::GetInstance()->party.size())
				break;
			modelStack.PushMatrix();
			modelStack.Translate(-70.3f + (i * 47), 50, 2);
			modelStack.Scale(15, 15 * 16 / 9, 1);
			RenderHelper::RenderMesh(PlayerInfo::GetInstance()->party.at(i)->getPortrait());
			modelStack.PopMatrix();
			counter++;
		}
	}
	else if (currentScreen == CURR_SCREEN_VIEW_STATUS) // Viewing status screen
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -1);
		modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("statusScreen"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(statusCursorPos.x, statusCursorPos.y, 10);
		modelStack.Scale(20,10,1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("rightFacingArrow"));
		modelStack.PopMatrix();

		if (PlayerInfo::GetInstance()->party.size() >= 4 && PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->b_inParty == false)
		{
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Party is full!", Vector3(-5.f, -40.f, 1.f), 17.f, Color(1, 1, 1));
		}
		else
		{
			if (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->b_inParty == false)
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Add to Party", Vector3(-5.f, -40.f, 1.f), 17.f, Color(1, 1, 1));
			else
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Remove from Party", Vector3(-5.f, -40.f, 1.f), 17.f, Color(1, 1, 1));
		}
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Back", Vector3(-5.f, -70.f, 1.f), 17.f, Color(1, 1, 1));
		// This character's portrait
		modelStack.PushMatrix();
		modelStack.Translate(-camera.f_OrthoSize + 37.5f, 0, 1);
		modelStack.Scale(50, 50 * 16/9, 1);
		RenderHelper::RenderMesh(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getPortrait());
		modelStack.PopMatrix();

		// This character's STR
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getSTR()), Vector3(-7.5f, 54, 1), 10.f, Color(1, 1, 1));
		// This character's DEX
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getDEX()), Vector3(-7.5f, 37, 1), 10.f, Color(1, 1, 1));
		// This character's LUK
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getLUK()), Vector3(-7.5f, 20, 1), 10.f, Color(1, 1, 1));
		// This character's HP
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getHP()), Vector3(-7.5f, 3, 1), 10.f, Color(1, 1, 1));

		// This character's Attack Range
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->i_attackRange), Vector3(55.5f, 54, 1), 10.f, Color(1, 1, 1));
		// This character's Movement Range
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->i_movementCost), Vector3(55.5f, 37, 1), 10.f, Color(1, 1, 1));
		// This character's weapon's name
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getWeapon()->s_Name), Vector3(55.5f, 20, 1), 10.f, Color(1, 1, 1));
		// This character's armor's name
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getArmor()->s_Name), Vector3(55.5f, 3, 1), 10.f, Color(1, 1, 1));
		// This character's name
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getName()), Vector3(-89, 69, 1), 15.f, Color(1, 1, 1));
	}
	else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -1);
		modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("changeWhoseEQ"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(selectedPos.x, selectedPos.y, 1);
		modelStack.Scale(15, 15 * 16 / 9, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selected"));
		modelStack.PopMatrix();

		for (int y = 1, counter = 0; y >= -32; y--)
		{
			for (int x = 0; x < 5; x++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(-60 + (x * 30), (y * 40), 0);
				modelStack.PushMatrix();
				modelStack.Scale(15, 15 * 16 / 9, 1);
				RenderHelper::RenderMesh(PlayerInfo::GetInstance()->availableUnits.at(counter)->getPortrait());
				modelStack.PopMatrix();
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), PlayerInfo::GetInstance()->availableUnits.at(counter)->getName(), Vector3(-11, -17, 0), 7.f, Color(1, 1, 1));
				modelStack.PopMatrix();
				counter++;
				if (counter >= PlayerInfo::GetInstance()->availableUnits.size())
					break;
			}
			if (counter >= PlayerInfo::GetInstance()->availableUnits.size())
				break;
		}
	}
}

void PartySelectScene::Exit()
{
	// Detach camera from other entities
	BGM->drop();
	GraphicsManager::GetInstance()->DetachCamera();
	groundEntity->SetIsDone(true);
	//playerInfo->DetachCamera();

	//	if (playerInfo->DropInstance() == false)
	//	{
	//#if _DEBUGMODE==1
	//		cout << "Unable to drop PlayerInfo class" << endl;
	//#endif
	//	}

	// Delete the lights
	GraphicsManager::GetInstance()->RemoveLight("lights[0]");
	GraphicsManager::GetInstance()->RemoveLight("lights[1]");
}
