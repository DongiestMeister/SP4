#include "MapController.h"
#include "KeyboardController.h"

MapController::MapController()
{
	selectedTile.Set(0, 0);
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
		if (map->GetCharacter(selectedTile.x,selectedTile.y).x >= 0)
		{
			std::cout << "Unit found!" << std::endl;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
	{
		if (selectedTile.y < map->numTilesHeight - 1)
		{
			selectedTile.y++;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
	{
		if (selectedTile.y > 0)
		{
			selectedTile.y--;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
	{
		if (selectedTile.x > 0)
		{
			selectedTile.x--;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
	{
		if (selectedTile.x < map->numTilesWidth - 1)
		{
			selectedTile.x++;
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