#include "SceneText.h"
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
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"
#include "WeaponInfo\Pistol.h"
#include "WeaponInfo\GrenadeThrow.h"
#include "WeaponInfo\LaserBlaster.h"
#include "RenderHelper.h"


#include <iostream>
using namespace std;

//SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

int GenerateOBJs(lua_State *L)
{
	int n = lua_gettop(L);

	string name, filepath;

	if (n == 2)
	{
		name = lua_tostring(L, 1);
		filepath = lua_tostring(L, 2);
	}

	MeshBuilder::GetInstance()->GenerateOBJ(name, filepath);

	return 0;
}

int SetTextures(lua_State *L)
{
	int n = lua_gettop(L);

	string name, filepath;

	if (n == 2)
	{
		name = lua_tostring(L, 1);
		filepath = lua_tostring(L, 2);
	}

	MeshBuilder::GetInstance()->GetMesh(name)->textureID = LoadTGA(filepath.c_str());

	return 0;
}

SceneText::SceneText()
{
}

//SceneText::SceneText(SceneManager* _sceneMgr)
//{
//	_sceneMgr->AddScene("Start", this);
//}

SceneText::~SceneText()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{
	died = false;

	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	Music::GetInstance()->Init();

	BGM = Music::GetInstance()->playSound("Sounds//bossfight.mp3", true, false, true);
	BGM->setVolume(0.3);
	
	m_iScore = 0;

	spawnEnemyNext = false;

	enemySpawnTimer = 5.f;

	waypoints[0] = CLuaInterface::GetInstance()->GetVector3("WaypointOne");
	waypoints[1] = CLuaInterface::GetInstance()->GetVector3("WaypointTwo");
	waypoints[2] = CLuaInterface::GetInstance()->GetVector3("WaypointThree");
	waypoints[3] = CLuaInterface::GetInstance()->GetVector3("WaypointFour");

	srand(NULL);
	// Tell the shader program to store these uniform locations
	/*currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");*/
	
	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 1, 1);
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
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	playerInfo->lives = 3;

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
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
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//Grassydirt.tga");
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

	lua_State *L = CLuaInterface::GetInstance()->theOBJState;
	luaL_openlibs(L);
	lua_register(L, "GenerateOBJ", &GenerateOBJs);
	lua_register(L, "SetTexture", &SetTextures);
	luaL_dofile(L, "Image//OBJs.lua");
	lua_close(L);

	//luaL_openlibs(L);
	//lua_register(L, "GenerateOBJ", &CLuaInterface::GetInstance()->GenerateOBJs);
	//lua_register(L, "SetTexture", &CLuaInterface::GetInstance()->SetTextures);
	//luaL_dofile(L, "Image//OBJs.lua");
	//lua_close(L);

	/*
	// Making a gun
	MeshBuilder::GetInstance()->GenerateOBJ("Pistol", "OBJ//Gun.obj");
	MeshBuilder::GetInstance()->GetMesh("Pistol")->textureID = LoadTGA("Image//GunUV.tga");

	//Cylinder
	MeshBuilder::GetInstance()->GenerateOBJ("Cylinder", "OBJ//Cylinder.obj");
	MeshBuilder::GetInstance()->GetMesh("Cylinder")->textureID = LoadTGA("Image//metal_texture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Cylinder_M", "OBJ//Cylinder_med.obj");
	MeshBuilder::GetInstance()->GetMesh("Cylinder_M")->textureID = LoadTGA("Image//metal_texture.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Cylinder_L", "OBJ//Cylinder_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Cylinder_L")->textureID = LoadTGA("Image//metal_texture.tga");

	// Making grass
	MeshBuilder::GetInstance()->GenerateOBJ("Plant", "OBJ//Plant.obj");
	MeshBuilder::GetInstance()->GetMesh("Plant")->textureID = LoadTGA("Image//Plant.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Plant_M", "OBJ//Plant_med.obj");
	MeshBuilder::GetInstance()->GetMesh("Plant_M")->textureID = LoadTGA("Image//Plant.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Plant_L", "OBJ//Plant_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Plant_L")->textureID = LoadTGA("Image//Plant.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Tree", "OBJ//Tree.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree")->textureID = LoadTGA("Image//Tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_M", "OBJ//Tree_med.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_M")->textureID = LoadTGA("Image//Tree.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tree_L", "OBJ//Tree_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree_L")->textureID = LoadTGA("Image//Tree.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Head", "OBJ//head.obj");
	MeshBuilder::GetInstance()->GetMesh("Head")->textureID = LoadTGA("Image//Head.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Head_M", "OBJ//head_med.obj");
	MeshBuilder::GetInstance()->GetMesh("Head_M")->textureID = LoadTGA("Image//Head.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Head_L", "OBJ//head_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Head_L")->textureID = LoadTGA("Image//Head.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Tent", "OBJ//tent.obj");
	MeshBuilder::GetInstance()->GetMesh("Tent")->textureID = LoadTGA("Image//tent.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tent_M", "OBJ//tent_med.obj");
	MeshBuilder::GetInstance()->GetMesh("Tent_M")->textureID = LoadTGA("Image//tent.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Tent_L", "OBJ//tent_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Tent_L")->textureID = LoadTGA("Image//tent.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Rock", "OBJ//Rock.obj");
	MeshBuilder::GetInstance()->GetMesh("Rock")->textureID = LoadTGA("Image//Rock.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Rock_M", "OBJ//Rock_med.obj");
	MeshBuilder::GetInstance()->GetMesh("Rock_M")->textureID = LoadTGA("Image//Rock.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Rock_L", "OBJ//Rock_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Rock_L")->textureID = LoadTGA("Image//Rock.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("Sail", "OBJ//sail.obj");
	MeshBuilder::GetInstance()->GetMesh("Sail")->textureID = LoadTGA("Image//sails.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Sail_M", "OBJ//sail_med.obj");
	MeshBuilder::GetInstance()->GetMesh("Sail_M")->textureID = LoadTGA("Image//sails.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Sail_L", "OBJ//sail_low.obj");
	MeshBuilder::GetInstance()->GetMesh("Sail_L")->textureID = LoadTGA("Image//sails.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("grenade", "OBJ//Grenade.obj");
	MeshBuilder::GetInstance()->GetMesh("grenade")->textureID = LoadTGA("Image//Grenade.tga");*/

	CWeaponInfo *primaryWeapon = new CPistol();
	primaryWeapon->Init();
	primaryWeapon->SetMaxMagRound(CLuaInterface::GetInstance()->getIntValue("gun_ammo"));
	primaryWeapon->SetMagRound(primaryWeapon->GetMaxMagRound());
	primaryWeapon->mesh = MeshBuilder::GetInstance()->GetMesh("Pistol");
	playerInfo->SetPrimaryWeapon(primaryWeapon);
	// Set the laser blaster as the secondary weapon
	//secondaryWeapon = new CLaserBlaster();
	//secondaryWeapon->Init();
	CWeaponInfo *secondaryWeapon = new CGrenadeThrow();
	secondaryWeapon->Init();
	playerInfo->SetSecondaryWeapon(secondaryWeapon);

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(200000, 300000);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	//Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	//Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball


	// Grass
	//GenericEntity* grass = Create::Entity("Grass_object", Vector3(0.0f, -10.0f, 0.0f),Vector3(5,5,5)); // Grass

	// Add the pointer to this new entity to the Scene Graph


	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(1000.0f, 1000.0f, 1000.0f));
	groundEntity->SetGrids(Vector3(1.0f, 1.0f, 1.0f));
	playerInfo->SetTerrain(groundEntity);

	for (int i = 0; i < 10; ++i)
	{
		float extraScale = rand() % 5;
		GenericEntity *tree = Create::Entity("Tree", Vector3(-400, -20.f - extraScale, -500 + (i * 100)), Vector3(15 + extraScale, 15 + extraScale, 15 + extraScale));
		tree->InitLOD("Tree", "Tree_M", "Tree_L");
	}

	// Create plants
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			float scale = rand() % 5 + 1;
			int rock = rand() % 2;
			if (rock == 0)
			{
				GenericEntity* grass = Create::Entity("Plant", Vector3(i * (30)*scale - 450, -10.f, j * (30)*scale - 450), Vector3(scale, scale, scale));
				grass->InitLOD("Plant", "Plant_M", "Plant_L");
			}
			else
			{
				GenericEntity* grass = Create::Entity("Rock", Vector3(i * (30)*scale - 450, -10.f, j * (30)*scale - 450), Vector3(scale, scale, scale));
				grass->InitLOD("Rock", "Rock_M", "Rock_L");
			}
			
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		GenericEntity* tent = Create::Entity("Tent", Vector3(400, -15.f, -300 + i*300), Vector3(5, 5, 5));
		tent->InitLOD("Tent", "Tent_M", "Tent_L");
	}

	// Create turret

	turret = new CTurret();
	turret->SetPosition(Vector3(0, -10.f, 400.f));
	turret->Init();

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 6; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	textObj[0]->SetPosition(Vector3(halfWindowWidth * 0.75 - fontSize * 2, -halfWindowHeight * 0.75 + fontSize + halfFontSize, 0.0f));
	textObj[0]->SetScale(Vector3(fontSize * 2, fontSize * 2, fontSize * 2));

	textObj[3]->SetPosition(Vector3(-halfWindowWidth * 0.25 - fontSize * 2, halfWindowHeight * 0.75 + fontSize + halfFontSize, 0.0f));
	textObj[3]->SetScale(Vector3(fontSize * 2, fontSize * 2, fontSize * 2));

	textObj[4]->SetPosition(Vector3(-halfWindowWidth * 0.15 - fontSize * 2, halfWindowHeight * 0.65 + fontSize + halfFontSize, 0.0f));

	textObj[5]->SetPosition(Vector3(-halfWindowWidth * 0.9 - fontSize * 2, -halfWindowHeight * 0.75 + fontSize + halfFontSize, 0.0f));
	textObj[5]->SetScale(Vector3(fontSize * 2, fontSize * 2, fontSize * 2));
}

void SceneText::Update(double dt)
{
	if (spawnEnemyNext)
	{
		spawnEnemyNext = false;
		CEnemy* enemy = SpawnEnemy();	
		for (int i = 0; i < 4; ++i)
		{
			enemy->AddWaypoint(waypoints[i]);
		}
	}
	
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	turret->Update(dt);

	for (vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end();)
	{
		(*it)->Update(dt);

		if (!CSceneGraph::GetInstance()->GetNode((*it)->Torso))
		{
				//playerInfo->lives--;
			if (playerInfo->lives > 0)
			{
				m_iScore++;
			}
			delete *it;
			it = enemyList.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (!died && playerInfo->lives == 0)
	{
		died = true;
		BGM->setIsPaused(true);
		CLuaInterface::GetInstance()->SetHighscore(m_iScore);
		Music::GetInstance()->playSound("Sounds//death.mp3");
	}


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
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('F'))
	{
		CSpatialPartition::GetInstance()->overrideDetails.SetDetailLevel(CLevelOfDetails::LOW_DETAILS);
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('G'))
	{
		CSpatialPartition::GetInstance()->overrideDetails.SetDetailLevel(CLevelOfDetails::MID_DETAILS);
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('H'))
	{
		CSpatialPartition::GetInstance()->overrideDetails.SetDetailLevel(CLevelOfDetails::HIGH_DETAILS);
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('B'))
	{
		CSpatialPartition::GetInstance()->overrideDetails.SetDetailLevel(CLevelOfDetails::NO_DETAILS);
	}

	if (KeyboardController::GetInstance()->IsKeyReleased('E'))
	{
		if (playerInfo->lives <= 0)
		{
			died = false;
			ClearEnemies();
			playerInfo->lives = 3;
			m_iScore = 0;
			BGM->setIsPaused(false);
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
	playerInfo->Update(dt);

	Music::GetInstance()->SetListener(playerInfo->GetPos(), playerInfo->GetPos() - playerInfo->GetTarget());

	if (playerInfo->lives > 0)
		enemySpawnTimer += (float)dt;

	if (enemySpawnTimer > 10.f)
	{
		enemySpawnTimer = 0.f;
		// Create a CEnemy instance
		if (enemyList.size() < 5)
		{
			spawnEnemyNext = true;
		}
	}

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[1]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[2]->SetText(ss1.str());

	ss1.str("");
	ss1 << playerInfo->GetPrimaryWeapon()->GetMagRound() << "/" << playerInfo->GetPrimaryWeapon()->GetMaxMagRound();
	textObj[0]->SetText(ss1.str());

	if (playerInfo->lives > 0)
	{
		ss1.str("");
		ss1 << "LIVES:" << (float)playerInfo->lives;
		textObj[3]->SetText(ss1.str());
		textObj[4]->SetText("");
	}
	else
	{
		textObj[3]->SetText("GAME OVER");
		textObj[4]->SetText("(E TO RESTART)");
	}

	ss1.str("");
	ss1 << "Score:" << m_iScore;
	textObj[5]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;

	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -2000, 2000);
	GraphicsManager::GetInstance()->DetachCamera();
	
	//Render weapon

	CWeaponInfo *weapon = playerInfo->GetPrimaryWeapon();

	RenderHelper::Render2DMesh(weapon->mesh, weapon->translation + weapon->translateOffset, weapon->scale, weapon->rotation + weapon->rotateOffset + Vector3(weapon->recoilOffset + weapon->reloadOffset + weapon->jumpOffset,0,0));

	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	BGM->drop();
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}

CEnemy* SceneText::SpawnEnemy()
{
	CEnemy* theEnemy = new CEnemy();
	theEnemy->SetTerrain(groundEntity);
	theEnemy->SetPos(Vector3(0.f, -0.5f, 0));
	theEnemy->Init();
	theEnemy->player = playerInfo;
	enemyList.push_back(theEnemy);
	return theEnemy;
}

void SceneText::ClearEnemies()
{
	for (vector<CEnemy*>::iterator it = enemyList.begin(); it != enemyList.end();)
	{
		delete *it;
		it = enemyList.erase(it);
	}
}

