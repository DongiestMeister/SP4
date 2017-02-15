#ifndef BATTLESCENE_H
#define BATTLESCENE_H

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
#include "PlayerInfo.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;

using std::vector;

class BattleScene : public Scene
{
public:
	BattleScene();
	~BattleScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RunBattleAnimation(double dt);
	void RenderProps();

private:
	ShaderProgram* currProg;
	//CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	//TextEntity* textObj[6];
	Light* lights[2];
	MeleeCharacter* player;

	float player_posx;
	float enemy_posx;
	bool b_isClashed;

	ISound* BGM;
};

#endif