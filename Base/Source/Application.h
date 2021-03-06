#ifndef APPLICATION_H
#define APPLICATION_H

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "timer.h"
#include "IntroState.h"
#include "MenuState.h"
#include "OptionState.h"
#include "GameplayScene.h"
#include "BattleScene.h"
#include "PartySelectScene.h"
#include "WarMapScene.h"
#include "RecruitScene.h"

class ShaderProgram;
struct GLFWwindow;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();

	void UpdateInput();
	void PostInputUpdate();
	
	static void MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset);

	int GetWindowHeight();
	int GetWindowWidth();

	bool ExitProgram;
	
private:
	Application();
	~Application();

	static bool IsKeyPressed(unsigned short key);
	void InitDisplay(void);

	// Should make these not hard-coded :P
	//const static int m_window_width = 1024;
	//const static int m_window_height = 800;

	int m_window_width;
	int m_window_height;

	//Declare a window object
	StopWatch m_timer;

	// Pointer to shader program
	ShaderProgram* currProg;
};

#endif