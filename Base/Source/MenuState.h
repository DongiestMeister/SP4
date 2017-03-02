#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "FPSCamera.h"
#include "SpriteEntity.h"
#include "TextEntity.h"
#include "Sounds\Music.h"

class SceneManager;

class CMenuState : public Scene
{
public:
	CMenuState();
	~CMenuState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	enum MENU_STATE
	{
		PLAY = 0,
		//OPTIONS,
		//HIGHSCORES,
		INSTRUCTIONS,
		EXIT,
		STATE_TOTAL
	};

	FPSCamera camera;
	SpriteEntity* MenuStateBackground;
	SpriteEntity* arrow;

	MENU_STATE state;

	ISound* BGM;

	bool b_renderInstructions;
};