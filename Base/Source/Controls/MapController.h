#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H
#include "Vector2.h"
#include "../TileMap.h"
#include "../FPSCamera.h"
#include "../AStar/AStar.h"

class MapController
{
public:
	MapController();
	~MapController();

	void Init(TileMap *map,FPSCamera *camera, bool *b_playerTurn);
	void Update(double dt);
	void Render();
	void RenderUI();

	Vector2 selectedTile;
	Character *selectedUnit;

	bool b_cameraTransition;
	bool b_forceCamera;
	float tempOrtho;

	bool *b_playerTurn;
private:
	TileMap *map;
	FPSCamera *camera;
	void PanTo(float speed,Vector2 pos, double dt);
	void GetUnitPath();

	void MoveUnit(float speed, double dt);
	vector<Vector2> unitPath;

	bool b_canPlace;
	bool b_movingUnit;
	bool b_attacking;
	void MovementControls(double dt);

	float f_upDebounce, f_downDebounce, f_leftDebounce, f_rightDebounce;
	float f_movementDelay; // delay in between movement of tiles when holding down

	vector<Character*> attackableUnits; // units within range of enemy
	Character *selectedEnemy; // the enemy that is selected to attack
	int enemyIterator;

	vector<Vector2> atkRangeIndicator;

	bool b_isZoomed;
	bool b_Zooming;

	enum CTRLBUTTONS
	{
		ATTACK,
		MOVE,
		WAIT,
		CTRL_TOTAL
	};

	void OpenButtons();
	void CloseButtons();
	void ButtonControls();
	void AttackControls();

	bool b_activeButtons[CTRL_TOTAL];
	CTRLBUTTONS currentButton;
	CTRLBUTTONS selectedButton;
};

#endif