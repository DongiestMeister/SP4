#include "MapController.h"
#include "KeyboardController.h"

MapController::MapController()
{
	selectedTile.Set(0, 0);
	selectedUnit = nullptr;
	b_movingUnit = false;
}

MapController::~MapController()
{

}

void MapController::Init(TileMap *map, FPSCamera *camera)
{
	this->map = map;
	this->camera = camera;
}

void MapController::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
	{
		if (!selectedUnit)
		{
			if (map->GetCharacter(selectedTile.x, selectedTile.y))
			{
				std::cout << "Unit found!" << std::endl;
				selectedUnit = map->GetCharacter(selectedTile.x, selectedTile.y);
			}
		}
		else if (b_canPlace)
		{
			unitPath = map->movePath;
			b_movingUnit = true;
			b_canPlace = false;
		}
	}

	if (b_movingUnit)
	{
		MoveUnit(5.f, dt);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
	{
		if (selectedTile.y < map->numTilesHeight - 1)
		{
			selectedTile.y++;
			GetUnitPath();
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
	{
		if (selectedTile.y > 0)
		{
			selectedTile.y--;
			GetUnitPath();
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
	{
		if (selectedTile.x > 0)
		{
			selectedTile.x--;
			GetUnitPath();
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
	{
		if (selectedTile.x < map->numTilesWidth - 1)
		{
			selectedTile.x++;
			GetUnitPath();
		}
	}

	// Moving the camera to the selected tile
	Vector2 currentPos(camera->GetCameraPos().x, camera->GetCameraPos().z);
	Vector2 targetPos(selectedTile.x * map->tileSizeX + map->tileSizeX / 2, selectedTile.y * map->tileSizeY + map->tileSizeY / 2);
	float Length = (currentPos - targetPos).Length();
	if (Length > 3)
	{
		PanTo(100.f, targetPos, dt);
	}
	else
	{
		camera->SetCameraPos(Vector3(targetPos.x, camera->GetCameraPos().y, targetPos.y)); // Snapping the camera to the grid
		camera->SetCameraTarget(Vector3(targetPos.x, camera->GetCameraTarget().y, targetPos.y)); // Snapping the camera to the grid
	}
}

void MapController::PanTo(float speed, Vector2 pos, double dt)
{
	Vector3 targetPos(selectedTile.x * map->tileSizeX + map->tileSizeX / 2,0, selectedTile.y * map->tileSizeY + map->tileSizeY / 2);
	Vector3 vel = (targetPos - camera->GetCameraPos()).Normalized() * speed * (float)dt;
	camera->SetCameraPos(camera->GetCameraPos() + vel);
	camera->SetCameraTarget(camera->GetCameraTarget() + vel);
}

void MapController::GetUnitPath()
{
	if (selectedUnit)
	{
		if ((selectedTile.x != (int)selectedUnit->pos.x || selectedTile.y != (int)selectedUnit->pos.y) && map->theScreenMap[selectedTile.y][selectedTile.x] == 0)
		{
			AStar search((int)selectedUnit->pos.x, (int)selectedUnit->pos.y, selectedTile.x, selectedTile.y, map);
			if ((selectedUnit->pos - Vector2(selectedTile.x, selectedTile.y)).Length() > selectedUnit->character->i_movementCost)
			{
				b_canPlace = false;
			}
			else if (search.Search())
			{
				if (search.bestPath.size() <= (selectedUnit->character->i_movementCost + 1))
				{
					selectedUnit->character->i_stepsTaken = search.bestPath.size() - 1;
					map->movePath = search.bestPath;
					b_canPlace = true;
				}
				else
				{
					b_canPlace = false;
				}
			}
			else
			{
				map->movePath.clear();
				b_canPlace = false;
			}
		}
		else
		{
			map->movePath.clear();
			b_canPlace = false;
		}
	}
}

void MapController::MoveUnit(float speed,double dt)
{
	if (selectedUnit)
	{
		if (unitPath.size() > 0)
		{
			if (!(unitPath[0] - selectedUnit->pos).IsZero())
			{
				Vector2 vel = (unitPath[0] - selectedUnit->pos).Normalized() * speed * dt;
				selectedUnit->pos = selectedUnit->pos + vel;
				selectedTile = selectedUnit->pos;
			}
			if ((unitPath[0] - selectedUnit->pos).Length() < 0.1f)
			{
				selectedUnit->pos = unitPath[0];
				selectedTile = selectedUnit->pos;
				unitPath.erase(unitPath.begin());
			}
		}
		else
		{
			selectedUnit = nullptr;
			b_movingUnit = false;
			map->movePath.clear();
		}
	}
}