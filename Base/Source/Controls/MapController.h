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

	void Init(TileMap *map,FPSCamera *camera);
	void Update(double dt);
	void Render();
	void RenderUI();

	Vector2 selectedTile;
	Character *selectedUnit;
private:
	TileMap *map;
	FPSCamera *camera;
	void PanTo(float speed,Vector2 pos, double dt);
	void GetUnitPath();

	void MoveUnit(float speed, double dt);
	vector<Vector2> unitPath;

	bool b_canPlace;
	bool b_movingUnit;
	void MovementControls(double dt);

	float f_upDebounce, f_downDebounce, f_leftDebounce, f_rightDebounce;
	float f_movementDelay; // delay in between movement of tiles when holding down

	enum CTRLBUTTONS
	{
		ATTACK,
		MOVE,
		WAIT,
		CTRL_TOTAL
	};

	void OpenButtons();
	void CloseButtons();

	bool b_activeButtons[CTRL_TOTAL];
	CTRLBUTTONS currentButton;
	CTRLBUTTONS selectedButton;
};

#endif