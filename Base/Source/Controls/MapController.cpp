#include "MapController.h"
#include "KeyboardController.h"

MapController::MapController()
{
	selectedTile.Set(0, 0);
	selectedUnit = nullptr;
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
		if (map->GetCharacter(selectedTile.x,selectedTile.y))
		{
			std::cout << "Unit found!" << std::endl;
			selectedUnit = map->GetCharacter(selectedTile.x, selectedTile.y);
		}
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
		if ((selectedTile.x != selectedUnit->pos.x || selectedTile.y != selectedUnit->pos.y) && map->theScreenMap[selectedTile.y][selectedTile.x] == 0)
		{
			AStar search(selectedUnit->pos.x, selectedUnit->pos.y, selectedTile.x, selectedTile.y, map);
			if (search.Search())
			{
				if (search.bestPath.size() < selectedUnit->character->i_movementCost + 1)
				{
					selectedUnit->character->i_stepsTaken = search.bestPath.size();
					map->movePath = search.bestPath;
				}		
			}
			else
			{
				map->movePath.clear();
			}
		}
		else
		{
			map->movePath.clear();
		}
	}
}