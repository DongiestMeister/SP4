#ifndef GAMEPLAYSCENE_H
#define GAMEPLAYSCENE_H

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
#include "TileMap.h"
#include "AStar\AStar.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;

using std::vector;

class GameplayScene : public Scene
{	
public:
	GameplayScene();
	~GameplayScene();

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

	TileMap gameMap;
};

#endif