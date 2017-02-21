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
	currProg = GraphicsManager::GetInstance()->GetActiveShader();

	BGM = Music::GetInstance()->playSound("Sounds//bossfight.mp3", true, false, true);
	BGM->setVolume(0.3);

	country = SINGAPORE;

	srand(NULL);

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

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 5.0f;
	float halfFontSize = fontSize / 2.0f;

	fps = Create::Text2DObject("text", Vector3(-100, -95, 1.0f), "", Vector3(fontSize, fontSize, fontSize), Color(1.0f, 1.0f, 1.0f));

}

void WarMapScene::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	LightMouseControl(dt);

	GraphicsManager::GetInstance()->UpdateLights(dt);

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps2 = (float)(1.f / dt);
	ss << "FPS: " << fps2;
	//ss << "FPS: " << camera.GetCameraPos();
	fps->SetText(ss.str());
}

void WarMapScene::LightMouseControl(double dt)
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
	modelStack.Translate(-25, 0, 0.1);
	modelStack.Scale(120, 200, 200);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Singapore"));
	modelStack.PopMatrix();
	RenderBackground();
	EntityManager::GetInstance()->RenderUI();
}

void WarMapScene::Exit()
{
	// Detach camera from other entities
	BGM->stop();
	BGM->drop();
	BGM = nullptr;

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
	//GraphicsManager::GetInstance()->RemoveLight("lights[0]");
	//GraphicsManager::GetInstance()->RemoveLight("lights[1]");
}

void WarMapScene::Pause()
{
	BGM->setIsPaused(true);
	fps->SetIsDone(true);
}

void WarMapScene::Resume()
{
	BGM->setIsPaused(false);

	float fontSize = 5.0f;

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