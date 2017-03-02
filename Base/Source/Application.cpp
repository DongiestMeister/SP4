#include "Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "Sounds\Music.h"
#include "EntityManager.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "SpriteAnimation.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "Lua/LuaInterface.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
: m_window_width(1024)
, m_window_height(800)
, ExitProgram(false)
{
}

Application::~Application()
{
}

void Application::InitDisplay(void)
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	currProg->AddUniform("MVP");
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
	currProg->AddUniform("textColor");

	GraphicsManager::GetInstance()->SetActiveShader("default");

	currProg->UpdateInt("numLights", 0);
	currProg->UpdateInt("textEnabled", 0);

	MeshBuilder::GetInstance()->GenerateSpriteAnimation("Zombie", 1, 6, 1)->textureID = LoadTGA("Image//zombie.tga");
}


void Application::Init()
{
	CLuaInterface::GetInstance()->Init();

	m_window_width = CLuaInterface::GetInstance()->getIntValue("width",CLuaInterface::GetInstance()->theLuaState);
	m_window_height = CLuaInterface::GetInstance()->getIntValue("height", CLuaInterface::GetInstance()->theLuaState);

	m_window_width = 1920;
	m_window_height = 1080;

	//CLuaInterface::GetInstance()->Run();

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "Crusader Tactics", glfwGetPrimaryMonitor(), NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	// Init systems
	GraphicsManager::GetInstance()->Init();

	// Load all relevant info from csv
	PlayerInfo::GetInstance()->loadCharactersFromCSV("Image//Characters.csv");
	PlayerInfo::GetInstance()->loadWeaponsFromCSV("Image//Weapons.csv");
	PlayerInfo::GetInstance()->loadArmorFromCSV("Image//Armors.csv");
	PlayerInfo::GetInstance()->LoadPlayerCharacters();
	PlayerInfo::GetInstance()->gold = CLuaInterface::GetInstance()->getIntValue("gold", CLuaInterface::GetInstance()->thePlayerState);

	// Load meshs of characters
	MeshBuilder::GetInstance()->GenerateQuad("Lucy", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Lucy")->textureID = LoadTGA("Image//Characters//Portraits//Lucy.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Rasis", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Rasis")->textureID = LoadTGA("Image//Characters//Portraits//Rasis.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Corrine", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Corrine")->textureID = LoadTGA("Image//Characters//Portraits//Corrine.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Exdeath", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Exdeath")->textureID = LoadTGA("Image//Characters//Portraits//Exdeath.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Kain", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Kain")->textureID = LoadTGA("Image//Characters//Portraits//Kain.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Ossas", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Ossas")->textureID = LoadTGA("Image//Characters//Portraits//Ossas.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Sinon", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Sinon")->textureID = LoadTGA("Image//Characters//Portraits//Sinon.tga");
	MeshBuilder::GetInstance()->GenerateQuad("DeathGun", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("DeathGun")->textureID = LoadTGA("Image//Characters//Portraits//DeathGun.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Zero", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Zero")->textureID = LoadTGA("Image//Characters//Portraits//Zero.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Ray", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Ray")->textureID = LoadTGA("Image//Characters//Portraits//Ray.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Lucid", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Lucid")->textureID = LoadTGA("Image//Characters//Portraits//Lucid.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Kirito", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Kirito")->textureID = LoadTGA("Image//Characters//Portraits//Kirito.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Tsuna", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Tsuna")->textureID = LoadTGA("Image//Characters//Portraits//Tsuna.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Slime", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Slime")->textureID = LoadTGA("Image//Characters//Portraits//Slime.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Kotori", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Kotori")->textureID = LoadTGA("Image//Characters//Portraits//Kotori.tga");

	// Load Weapons
	MeshBuilder::GetInstance()->GenerateQuad("RainbowSword", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("RainbowSword")->textureID = LoadTGA("Image//itemPortraits//Weapons//RainbowSword.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Voltexes", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Voltexes")->textureID = LoadTGA("Image//itemPortraits//Weapons//Voltexes.tga");
	MeshBuilder::GetInstance()->GenerateQuad("YatoBlade", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("YatoBlade")->textureID = LoadTGA("Image//itemPortraits//Weapons//YatoBlade.tga");
	MeshBuilder::GetInstance()->GenerateQuad("ExSword", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("ExSword")->textureID = LoadTGA("Image//itemPortraits//Weapons//ExSword.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Gungnir", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Gungnir")->textureID = LoadTGA("Image//itemPortraits//Weapons//Gungnir.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Uvuvwevwevwe", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Uvuvwevwevwe")->textureID = LoadTGA("Image//itemPortraits//Weapons//Uvuvwevwevwe.tga");
	MeshBuilder::GetInstance()->GenerateQuad("PGMUltimaII", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("PGMUltimaII")->textureID = LoadTGA("Image//itemPortraits//Weapons//PGMUltimaII.tga");
	MeshBuilder::GetInstance()->GenerateQuad("T54BlackStar", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("T54BlackStar")->textureID = LoadTGA("Image//itemPortraits//Weapons//T54BlackStar.tga");
	MeshBuilder::GetInstance()->GenerateQuad("AkashaSword", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("AkashaSword")->textureID = LoadTGA("Image//itemPortraits//Weapons//AkashaSword.tga");
	MeshBuilder::GetInstance()->GenerateQuad("BrokenSword", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("BrokenSword")->textureID = LoadTGA("Image//itemPortraits//Weapons//BrokenSword.tga");
	MeshBuilder::GetInstance()->GenerateQuad("ButterflyBow", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("ButterflyBow")->textureID = LoadTGA("Image//itemPortraits//Weapons//ButterflyBow.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Elucidator", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Elucidator")->textureID = LoadTGA("Image//itemPortraits//Weapons//Elucidator.tga");
	MeshBuilder::GetInstance()->GenerateQuad("XGloves", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("XGloves")->textureID = LoadTGA("Image//itemPortraits//Weapons//XGloves.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GlobOfSlime", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("GlobOfSlime")->textureID = LoadTGA("Image//itemPortraits//Weapons//GlobOfSlime.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Camael", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Camael")->textureID = LoadTGA("Image//itemPortraits//Weapons//Camael.tga");
	
	// Load Armor
	MeshBuilder::GetInstance()->GenerateQuad("RainbowShield", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("RainbowShield")->textureID = LoadTGA("Image//itemPortraits//Armor//RainbowShield.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Seraphim", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Seraphim")->textureID = LoadTGA("Image//itemPortraits//Armor//Seraphim.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GodsHowl", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("GodsHowl")->textureID = LoadTGA("Image//itemPortraits//Armor//GodsHowl.tga");
	MeshBuilder::GetInstance()->GenerateQuad("ExArmor", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("ExArmor")->textureID = LoadTGA("Image//itemPortraits//Armor//ExArmor.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SkyGodPlate", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("SkyGodPlate")->textureID = LoadTGA("Image//itemPortraits//Armor//SkyGodPlate.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Onyetenvewve", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("Onyetenvewve")->textureID = LoadTGA("Image//itemPortraits//Armor//Onyetenvewve.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SniperArmor", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("SniperArmor")->textureID = LoadTGA("Image//itemPortraits//Armor//SniperArmor.tga");
	MeshBuilder::GetInstance()->GenerateQuad("DeathCloak", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("DeathCloak")->textureID = LoadTGA("Image//itemPortraits//Armor//DeathCloak.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GCloak", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("GCloak")->textureID = LoadTGA("Image//itemPortraits//Armor//GCloak.tga");
	MeshBuilder::GetInstance()->GenerateQuad("RustyArmor", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("RustyArmor")->textureID = LoadTGA("Image//itemPortraits//Armor//RustyArmor.tga");
	MeshBuilder::GetInstance()->GenerateQuad("BFlyOveralls", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("BFlyOveralls")->textureID = LoadTGA("Image//itemPortraits//Armor//BFlyOveralls.tga");
	MeshBuilder::GetInstance()->GenerateQuad("BlackwyrmCoat", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("BlackwyrmCoat")->textureID = LoadTGA("Image//itemPortraits//Armor//BlackwyrmCoat.tga");
	MeshBuilder::GetInstance()->GenerateQuad("VonGearX", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("VonGearX")->textureID = LoadTGA("Image//itemPortraits//Armor//VonGearX.tga");
	MeshBuilder::GetInstance()->GenerateQuad("SlimeBubble", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("SlimeBubble")->textureID = LoadTGA("Image//itemPortraits//Armor//SlimeBubble.tga");
	MeshBuilder::GetInstance()->GenerateQuad("ElohimGibor", Color(1, 1, 1));
	MeshBuilder::GetInstance()->GetMesh("ElohimGibor")->textureID = LoadTGA("Image//itemPortraits//Armor//ElohimGibor.tga");

	SceneManager::GetInstance()->AddScene("IntroState", new CIntroState());
	SceneManager::GetInstance()->AddScene("MenuState", new CMenuState());
	SceneManager::GetInstance()->AddScene("GameState", new GameplayScene());
	SceneManager::GetInstance()->AddScene("OptionState", new COptionState());
	SceneManager::GetInstance()->AddScene("BattleState", new BattleScene());
	SceneManager::GetInstance()->AddScene("PartySelect", new PartySelectScene());
	SceneManager::GetInstance()->AddScene("WarMap", new WarMapScene());
	SceneManager::GetInstance()->AddScene("RecruitState", new RecruitScene());
	//
	SceneManager::GetInstance()->SetActiveScene("IntroState");
}


void Application::Run()
{
	InitDisplay();

	//SceneManager::GetInstance()->SetActiveScene("Start");
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !ExitProgram)
	{
		glfwPollEvents();
		UpdateInput();

		SceneManager::GetInstance()->Update(m_timer.getElapsedTime());
		SceneManager::GetInstance()->Render();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...

		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

		PostInputUpdate();
	}
	PlayerInfo::GetInstance()->SavePlayerCharacters();
	CLuaInterface::GetInstance()->SaveIntValue("gold", PlayerInfo::GetInstance()->gold, true, "Image//Lua//Player.lua");
	CLuaInterface::GetInstance()->Destroy();
	SceneManager::GetInstance()->Destroy();
	PlayerInfo::GetInstance()->Destroy();

	EntityManager::GetInstance()->Destroy();
	MeshBuilder::GetInstance()->Destroy();
	GraphicsManager::GetInstance()->Destroy();
	Music::GetInstance()->Destroy();
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();

	//_CrtDumpMemoryLeaks();
}

void Application::UpdateInput()
{
	// Update Mouse Position
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);

	// Update Keyboard Input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::PostInputUpdate()
{
	// If mouse is centered, need to update the center position for next frame
	if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
		double mouse_currX, mouse_currY;
		MouseController::GetInstance()->GetMousePosition(mouse_currX, mouse_currY);
		//if (mouse_currX > m_window_width * 3/4 || mouse_currX < m_window_width/4)
		//{
		//	mouse_currX = m_window_width >> 1;
		//	glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
		//	MouseController::GetInstance()->DefaultMousePositions(mouse_currX, mouse_currY);
		//}
		//if (mouse_currY > m_window_height * 3 / 4 || mouse_currY < m_window_height / 4)
		//{
		//	mouse_currY = m_window_height >> 1;
		//	glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
		//	MouseController::GetInstance()->DefaultMousePositions(mouse_currX, mouse_currY);
		//}
	}

	// Call input systems to update at end of frame
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
}

void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

int Application::GetWindowHeight()
{
	return m_window_height;
}

int Application::GetWindowWidth()
{
	return m_window_width;
}