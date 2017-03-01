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

	enum CURRENT_SCREEN
	{
		CURR_SCREEN_SELECT_OPTION,
		CURR_SCREEN_SELECT_UNITS,
		CURR_SCREEN_VIEW_STATUS,
		CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER,
		CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ,
		CURR_SCREEN_SHOP,
	};
	CURRENT_SCREEN currentScreen;

	Vector3 selectedPos;

	int i_selectedUnitsCounter; // for units
	int i_selectedOptionCounter; // for going into screens (select characters/change equipment etc)
	int i_selectedEquipmentCounter;
	int i_shopCursor; // The actual items
	int i_selectedShopCounter; // Buy, sell, or back
	int i_eqToShow;
	bool b_statusCursor;
	bool b_equipmentCursor;
	bool b_showArrowAtEQs;
	bool b_showShopItems; // For buying from shop
	bool b_showPlayerItems; // To sell items to the shop
	bool b_showItemStatsAtShop;
	bool b_equipmentPageNumber;
	Vector3 statusCursorPos;
	Vector3 equipmentCursorPos;
	Vector3 shopCursorPos;
	ISound* BGM;
	string message;
};

#endif