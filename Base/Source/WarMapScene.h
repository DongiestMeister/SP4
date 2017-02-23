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
#include "Level.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class SpriteEntity;
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
	enum BUTTONS
	{
		PLAY,
		OPTIONS,
		RECRUIT,
		B_TOTAL
	};

	BUTTONS currentButton;
	SpriteEntity *select;

	ShaderProgram* currProg;
	FPSCamera camera;
	TextEntity* fps;

	Light* lights[2];

	ISound* BGM;

	void MouseControl(double dt);

	void RenderBackground();
	void RenderButtons();
	void RenderLevels();
	float f_buttonXoffset;

	enum CURRENT_COUNTRY
	{
		SINGAPORE,
		C_TOTAL
	};

	vector<Level> levelList;
	unsigned int levelIterator;

	void RandomLevels();

	CURRENT_COUNTRY country;
};

#endif