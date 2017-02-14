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

	Vector2 selectedTile;
	Unit *selectedUnit;
private:
	TileMap *map;
	FPSCamera *camera;
	void PanTo(float speed,Vector2 pos, double dt);
	void GetUnitPath();

	void MoveUnit(float speed, double dt);
	vector<Vector2> unitPath;

	bool b_canPlace;
	bool b_movingUnit;
};

#endif