#include "MapController.h"
#include "KeyboardController.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"

MapController::MapController()
{
	selectedTile.Set(0, 0);
	selectedUnit = nullptr;
	b_movingUnit = false;
	b_canPlace = false;
	f_upDebounce = f_downDebounce = f_leftDebounce = f_rightDebounce = 0.f;
	f_movementDelay = 0.2f;
	b_activeButtons[0] = false;
	b_activeButtons[1] = false;
	b_activeButtons[2] = false;
	currentButton = ATTACK;
	selectedButton = CTRL_TOTAL;
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
		if (!selectedUnit) // Selected a unit on the map
		{
			if (map->GetCharacter(selectedTile.x, selectedTile.y))
			{
				if (!map->GetCharacter(selectedTile.x, selectedTile.y)->character->b_tookAction)
				{
					std::cout << "Unit found!" << std::endl;
					selectedUnit = map->GetCharacter(selectedTile.x, selectedTile.y);
					OpenButtons();
				}
			}
		}
		else if (b_canPlace) // Moving the unit
		{
			unitPath = map->movePath;
			selectedUnit->character->i_stepsTaken += unitPath.size();
			b_movingUnit = true;
			b_canPlace = false;
		}
		else if (b_activeButtons[WAIT]) // If choosing a button
		{
			selectedButton = currentButton;
			CloseButtons();
		}
	}

	if (b_movingUnit)
	{
		MoveUnit(5.f, dt);
	}

	if (!b_activeButtons[WAIT])
	{
		MovementControls(dt);
	}

	if (b_activeButtons[WAIT]) // If buttons are active
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
		{
			if (currentButton == ATTACK)
			{
				currentButton = WAIT;
			}
			else if (currentButton == MOVE)
			{
				if (b_activeButtons[ATTACK])
				{
					currentButton = ATTACK;
				}
				else
				{
					currentButton = WAIT;
				}
			}
			else if (currentButton == WAIT)
			{
				if (b_activeButtons[MOVE])
				{
					currentButton = MOVE;
				}
				else if (b_activeButtons[ATTACK])
				{
					currentButton = ATTACK;
				}
			}
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
		{
			if (currentButton == WAIT)
			{
				if (b_activeButtons[ATTACK])
					currentButton = ATTACK;
				else if (b_activeButtons[MOVE])
					currentButton = MOVE;
			}
			else if (currentButton == ATTACK)
			{
				if (b_activeButtons[MOVE])
					currentButton = MOVE;
				else
					currentButton = WAIT;
			}
			else if (currentButton == MOVE)
			{
				currentButton = WAIT;
			}
		}
	}


	// Moving the camera to the selected tile
	Vector2 currentPos(camera->GetCameraPos().x, camera->GetCameraPos().z);
	Vector2 targetPos(selectedTile.x * map->tileSizeX + map->tileSizeX / 2, selectedTile.y * map->tileSizeY + map->tileSizeY / 2);
	float Length = (currentPos - targetPos).Length();
	if (Length > 3)
	{
		PanTo(camera->f_OrthoSize, targetPos, dt);
	}
	else
	{
		camera->SetCameraPos(Vector3(targetPos.x, camera->GetCameraPos().y, targetPos.y)); // Snapping the camera to the grid
		camera->SetCameraTarget(Vector3(targetPos.x, camera->GetCameraTarget().y, targetPos.y)); // Snapping the camera to the grid
	}
}

void MapController::PanTo(float speed, Vector2 pos, double dt)
{
	Vector3 targetPos(selectedTile.x * map->tileSizeX + map->tileSizeX / 2, 0, selectedTile.y * map->tileSizeY + map->tileSizeY / 2);
	Vector3 vel = (targetPos - camera->GetCameraPos()).Normalized() * speed * (float)dt;
	camera->SetCameraPos(camera->GetCameraPos() + vel);
	camera->SetCameraTarget(camera->GetCameraTarget() + vel);
}

void MapController::GetUnitPath()
{
	if (selectedUnit)
	{
		if (!map->GetCharacter(selectedTile.x, selectedTile.y) && map->theScreenMap[selectedTile.y][selectedTile.x] == 0)
		{
			AStar search((int)selectedUnit->character->getPos().x, (int)selectedUnit->character->getPos().y, selectedTile.x, selectedTile.y, map);
			if ((selectedUnit->character->getPos() - Vector2(selectedTile.x, selectedTile.y)).Length() > selectedUnit->character->i_movementCost)
			{
				b_canPlace = false;
			}
			else if (search.Search())
			{
				if (search.bestPath.size() <= (selectedUnit->character->i_movementCost - selectedUnit->character->i_stepsTaken + 1))
				{
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

void MapController::MoveUnit(float speed, double dt)
{
	if (selectedUnit)
	{
		if (unitPath.size() > 0)
		{
			if (!(unitPath[0] - selectedUnit->character->getPos()).IsZero())
			{
				Vector2 vel = (unitPath[0] - selectedUnit->character->getPos()).Normalized() * speed * dt;
				selectedUnit->character->setPos(selectedUnit->character->getPos() + vel);
				selectedTile = selectedUnit->character->getPos();
			}
			if ((unitPath[0] - selectedUnit->character->getPos()).Length() < 0.1f)
			{
				selectedUnit->character->setPos(unitPath[0]);
				selectedTile = selectedUnit->character->getPos();
				unitPath.erase(unitPath.begin());
			}
		}
		else
		{
			b_movingUnit = false;
			map->movePath.clear();
			OpenButtons();
		}
	}
}

void MapController::MovementControls(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
	{
		f_upDebounce -= (float)dt;
		if (f_upDebounce < 0)
		{
			if (!b_movingUnit)
			{
				if (selectedTile.y < map->numTilesHeight - 1)
				{
					selectedTile.y++;
					if (selectedButton == MOVE)
					{
						GetUnitPath();
					}
				}
			}
			f_upDebounce = f_movementDelay;
		}
	}
	else if (KeyboardController::GetInstance()->IsKeyUp(VK_UP))
	{
		f_upDebounce = 0.f;
	}

	if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
	{
		f_downDebounce -= (float)dt;
		if (f_downDebounce < 0)
		{
			if (!b_movingUnit)
			{
				if (selectedTile.y > 0)
				{
					selectedTile.y--;
					if (selectedButton == MOVE)
					{
						GetUnitPath();
					}
				}
			}
			f_downDebounce = f_movementDelay;
		}
	}
	else if (KeyboardController::GetInstance()->IsKeyUp(VK_DOWN))
	{
		f_downDebounce = 0.f;
	}

	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
	{
		f_leftDebounce -= (float)dt;
		if (f_leftDebounce < 0)
		{
			if (!b_movingUnit)
			{
				if (selectedTile.x > 0)
				{
					selectedTile.x--;
					if (selectedButton == MOVE)
					{
						GetUnitPath();
					}
				}
			}
			f_leftDebounce = f_movementDelay;
		}
	}
	else if (KeyboardController::GetInstance()->IsKeyUp(VK_LEFT))
	{
		f_leftDebounce = 0.f;
	}

	if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		f_rightDebounce -= (float)dt;
		if (f_rightDebounce < 0)
		{
			if (!b_movingUnit)
			{
				if (selectedTile.x < map->numTilesWidth - 1)
				{
					selectedTile.x++;
					if (selectedButton == MOVE)
					{
						GetUnitPath();
					}
				}
			}
			f_rightDebounce = f_movementDelay;
		}
	}
	else if (KeyboardController::GetInstance()->IsKeyUp(VK_RIGHT))
	{
		f_rightDebounce = 0.f;
	}
}

void MapController::OpenButtons()
{
	selectedButton = CTRL_TOTAL;

	const struct { int x, y; } succ[4] = { { 0, -1 }, { 0, 1 }, { 1, 0 }, { -1, 0 } };
	for (int i = 0; i < 4; ++i)
	{
		if (map->GetCharacter(selectedUnit->character->getPos().x + succ[i].x, selectedUnit->character->getPos().y + succ[i].y))
		{
			b_activeButtons[ATTACK] = true;
			break;
		}
	}

	if (selectedUnit->character->i_stepsTaken < selectedUnit->character->i_movementCost)
	{
		b_activeButtons[MOVE] = true;
	}

	b_activeButtons[WAIT] = true;

	if (b_activeButtons[ATTACK])
	{
		currentButton = ATTACK;
	}
	else if (b_activeButtons[MOVE])
	{
		currentButton = MOVE;
	}
	else
	{
		currentButton = WAIT;
	}
}

void MapController::CloseButtons()
{
	b_activeButtons[ATTACK] = false;
	b_activeButtons[MOVE] = false;
	b_activeButtons[WAIT] = false;

	if (selectedButton == WAIT || selectedButton == ATTACK)
	{
		selectedUnit->character->b_tookAction = true;
		selectedUnit = nullptr;
	}
}

void MapController::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	// Rendering selected tile
	if (!b_movingUnit)
	{
		modelStack.PushMatrix();
		modelStack.Translate(selectedTile.x * map->tileSizeX + map->tileSizeX / 2, -0.3, selectedTile.y * map->tileSizeY + map->tileSizeY / 2);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(map->tileSizeX, map->tileSizeY, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("SelectedArrow"));
		modelStack.PopMatrix();
	}
}

void MapController::RenderUI()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();


	float f_yOffset = 0.f;
	for (int i = 0; i < CTRL_TOTAL; ++i)
	{
		if (b_activeButtons[i])
		{
			modelStack.PushMatrix();
			modelStack.Translate(camera->f_OrthoSize / 5, f_yOffset, 1.f);
			f_yOffset -= camera->f_OrthoSize / 8;
			modelStack.Scale(camera->f_OrthoSize / 10 * 4 / 3, camera->f_OrthoSize / 10, 1.f);
			if ((int)currentButton == i)
			{
				modelStack.Scale(1.5f, 1.5f, 1.f);
			}
			if (i == ATTACK)
			{
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Attack"));
			}
			else if (i == MOVE)
			{
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Move"));
			}
			else if (i == WAIT)
			{
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Wait"));
			}
			modelStack.PopMatrix();
		}
	}
}