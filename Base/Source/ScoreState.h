#pragma once

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "FPSCamera.h"
#include "SpriteEntity.h"
#include "TextEntity.h"

class SceneManager;

class CScoreState : public Scene
{
public:
	CScoreState();
	~CScoreState();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	FPSCamera camera;
	SpriteEntity* ScoreStateBackground;
	TextEntity* textObj[3];

};