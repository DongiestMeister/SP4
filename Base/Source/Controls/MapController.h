#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H
#include "Vector2.h"
#include "../TileMap.h"
#include "../FPSCamera.h"

class MapController
{
public:
	MapController();
	~MapController();

	void Init(TileMap *map,FPSCamera *camera);
	void Update(double dt);

	Vector2 selectedTile;
private:
	TileMap *map;
	FPSCamera *camera;
	void PanTo(float speed,Vector2 pos, double dt);
};

#endif