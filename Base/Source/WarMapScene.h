#ifndef WARMAPSCENE_H
#define WARMAPSCENE_H

#include "Scene.h"
#include "Mtx44.h"
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

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;

using std::vector;

class WarMapScene : public Scene
{
public:
	WarMapScene();
	~WarMapScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Pause();
	virtual void Resume();

private:
	ShaderProgram* currProg;
	FPSCamera camera;
	TextEntity* fps;

	Light* lights[2];

	ISound* BGM;

	void LightMouseControl(double dt);

	void RenderBackground();

	enum CURRENT_COUNTRY
	{
		SINGAPORE,
		C_TOTAL
	};

	CURRENT_COUNTRY country;
};

#endif