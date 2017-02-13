#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "FPSCamera.h"
#include "SpriteEntity.h"
#include "TextEntity.h"

class SceneManager;

class COptionState : public Scene
{
public:
	COptionState();
	~COptionState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	enum OPTIONS
	{
		AUDIO = 0,
		VIDEO,
		INPUT
	};

private:

	FPSCamera camera;
	SpriteEntity* OptionStateBackground;
	TextEntity* textObj;
	SpriteEntity* arrow[2];

	OPTIONS state;

};