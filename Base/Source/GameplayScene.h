#ifndef GAMEPLAYSCENE_H
#define GAMEPLAYSCENE_H

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
#include "TileMap.h"
#include "AStar\AStar.h"
#include "Controls\MapController.h"
#include "Level.h"

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
	virtual void Pause();
	virtual void Resume();
private:
	float f_timeDelay;

	ShaderProgram* currProg;
	//CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	//TextEntity* textObj[6];
	TextEntity* fps;
	TextEntity* turnDisplay;

	Light* lights[2];

	MapController controller;

	bool b_playerTurn;
	int i_turn;

	ISound* BGM;

	TileMap gameMap;

	void DisplayText(string text,Vector3 color);


	void DisplayWin(bool win);
	bool b_renderWin;
	bool b_renderLose;
	Vector2 bannerPos;


	void LightMouseControl(double dt);

	bool b_textRunning;

	int i_enemyIterator;

	VICTORY_CONDITION condition;
	int i_surviveTurns;
};

#endif