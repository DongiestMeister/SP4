#ifndef EMPTYSCENE_H
#define EMPTYSCENE_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include <vector>
#include "Sounds/Music.h"
#include "Lua\LuaInterface.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;

using std::vector;

class EmptyScene : public Scene
{	
public:
	EmptyScene();
	~EmptyScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	ShaderProgram* currProg;
	//CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	//TextEntity* textObj[6];
	Light* lights[2];


	ISound* BGM;
};

#endif