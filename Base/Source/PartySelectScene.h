#ifndef PARTYSELECTSCENE_H
#define PARTYSELECTSCENE_H

#include "Scene.h"
#include "Mtx44.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include <vector>
#include "Sounds/Music.h"
#include "Lua\LuaInterface.h"
#include "Character/MeleeCharacter.h"
#include "Character/Weapon.h"
#include "Character/Armor.h"
#include "PlayerInfo.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;

using std::vector;

class PartySelectScene : public Scene
{
public:
	PartySelectScene();
	~PartySelectScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	ShaderProgram* currProg;
	GroundEntity* groundEntity;
	FPSCamera camera;
	//TextEntity* textObj[6];
	Light* lights[2];

	Vector3 selectedPos;
	int i_selectedCounter;

	bool b_showStatus;
	bool b_statusCursor;
	Vector3 statusCursorPos;
	ISound* BGM;
};

#endif