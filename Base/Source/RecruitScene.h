#ifndef RECRUIT_SCENE_H
#define RECRUIT_SCENE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "FPSCamera.h"
#include "SpriteEntity.h"
#include "Character/Character.h"
#include "Character/MeleeCharacter.h"
#include "Character/RangedCharacter.h"

class SceneManager;

class RecruitScene : public Scene
{
public:
	RecruitScene();
	~RecruitScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;

	bool b_showObtainedUnit;
	Character* newChar;
	string attackType;
	string message;
};

#endif 