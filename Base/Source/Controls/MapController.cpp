#include "MapController.h"
#include "KeyboardController.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"
#include "../PlayerInfo.h"
#include "SceneManager.h"

MapController::MapController()
{

}

MapController::~MapController()
{

}

void MapController::Init(TileMap *map, FPSCamera *camera,bool *b_playerTurn)
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
	b_activeButtons[3] = false;
	currentButton = ATTACK;
	selectedButton = CTRL_TOTAL;
	b_attacking = false;
	selectedEnemy = nullptr;
	enemyIterator = 0;
	b_cameraTransition = false;
	tempOrtho = 100.f;
	b_isZoomed = true;
	b_Zooming = false;

	this->map = map;
	this->camera = camera;
	this->b_playerTurn = b_playerTurn;
	this->b_forceCamera = false;
}

void MapController::Update(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
	{
		if (!selectedUnit && (*b_playerTurn)) // Selected a unit on the map
		{
			if (map->GetCharacter(selectedTile.x, selectedTile.y))
			{
				if (!map->GetCharacter(selectedTile.x, selectedTile.y)->b_tookAction)
				{
					//std::cout << "Unit found!" << std::endl;
					selectedUnit = map->GetCharacter(selectedTile.x, selectedTile.y);
					attackableUnits.clear();
					OpenButtons();
				}
			}
		}
		else if (b_canPlace) // Moving the unit
		{
			unitPath = map->movePath;
			map->theScreenMap[(int)selectedUnit->getPos().y][(int)selectedUnit->getPos().x] = 0;
			b_movingUnit = true;
			b_canPlace = false;
		}
		else if (b_activeButtons[WAIT]) // If choosing a button
		{
			selectedButton = currentButton;
			CloseButtons();
		}
		else if (b_attacking)
		{
			if (selectedButton == SPECIAL)
			{
				PlayerInfo::GetInstance()->b_bonus = true;
				selectedUnit->i_specialMeter = 0;
			}
			else
			{
				if (selectedUnit->i_specialMeter + 20 <= 100)
					selectedUnit->i_specialMeter += 20;
				else
					selectedUnit->i_specialMeter = 100;
			}
			b_forceCamera = false;
			b_attacking = false;
			//selectedUnit->b_tookAction = true;
			PlayerInfo::GetInstance()->enemy = selectedEnemy;
			PlayerInfo::GetInstance()->player = selectedUnit;
			PlayerInfo::GetInstance()->b_attacking = true;
			selectedEnemy = nullptr;
			selectedUnit = nullptr;
			attackableUnits.clear();
			tempOrtho = camera->f_OrthoSize;
			b_cameraTransition = true;
			enemyIterator = 0;

			//SceneManager::GetInstance()->SetActiveScene("BattleState", true);
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('X'))
	{
		if (b_attacking)
		{
			b_attacking = false;
			b_forceCamera = false;
			selectedTile = selectedUnit->getPos();
			OpenButtons();
		}
		else if (selectedUnit && selectedButton == MOVE && !b_movingUnit)
		{
			selectedTile = selectedUnit->getPos();
			selectedUnit->i_stepsTaken = 0;
			map->movePath.clear();
			b_canPlace = false;
			OpenButtons();
		}
		else if (b_activeButtons[WAIT])
		{
			selectedUnit = nullptr;
			CloseButtons();
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('C'))
	{
		if (!b_Zooming)
		{
			b_Zooming = true;
			b_isZoomed = !b_isZoomed;
		}
	}

	if (!b_cameraTransition)
	{
		if (b_Zooming)
		{
			if (b_isZoomed)
			{
				if ((camera->f_OrthoSize -= 200 * dt) <= 100)
				{
					camera->f_OrthoSize = 100.f;
					b_Zooming = false;
				}
			}
			else if (!b_isZoomed)
			{
				if ((camera->f_OrthoSize += 200 * dt) >= 200)
				{
					camera->f_OrthoSize = 200.f;
					b_Zooming = false;
				}
			}
		}
	}

	if (b_cameraTransition)
	{
		camera->f_OrthoSize -= 100 * dt;
		if (camera->f_OrthoSize < 10)
		{
			SceneManager::GetInstance()->SetActiveScene("BattleState", true);
		}
	}

	if (b_movingUnit)
	{
		MoveUnit(5.f, dt);
	}

	if (!b_activeButtons[WAIT] && !b_attacking && (*b_playerTurn))
	{
		MovementControls(dt);
	}

	ButtonControls();

	if (b_attacking)
	{
		AttackControls();
	}


	// Moving the camera to the selected tile
	if (!b_cameraTransition)
	{
		Vector2 currentPos(camera->GetCameraPos().x, camera->GetCameraPos().z);
		Vector2 targetPos(selectedTile.x * map->tileSizeX + map->tileSizeX / 2, selectedTile.y * map->tileSizeY + map->tileSizeY / 2);
		//float Length = (currentPos - targetPos).Length();
		//if (Length > 50)
		//{
		//	if (Length > 3)
		//	{
		//		PanTo(camera->f_OrthoSize, targetPos, dt);
		//	}
		//	else
		//	{
		//		camera->SetCameraPos(Vector3(targetPos.x, camera->GetCameraPos().y, targetPos.y)); // Snapping the camera to the grid
		//		camera->SetCameraTarget(Vector3(targetPos.x, camera->GetCameraTarget().y, targetPos.y)); // Snapping the camera to the grid
		//	}
		//}
		if (!b_forceCamera)
		{
			if (abs(currentPos.y - targetPos.y) > 30) // Distance to start panning camera
			{
				Vector2 newTarget;
				if (currentPos.y - targetPos.y < 0)
				{
					newTarget = Vector2(camera->GetCameraPos().x, camera->GetCameraPos().z - (currentPos.y - targetPos.y + 30));
				}
				else
				{
					newTarget = Vector2(camera->GetCameraPos().x, camera->GetCameraPos().z - (currentPos.y - targetPos.y - 30));
				}
				PanTo(100, newTarget, dt);
			}
			if (abs(currentPos.x - targetPos.x) > 30) // Distance to start panning camera
			{
				Vector2 newTarget;
				if (currentPos.x - targetPos.x < 0)
				{
					newTarget = Vector2(camera->GetCameraPos().x - (currentPos.x - targetPos.x + 30), camera->GetCameraPos().z);
				}
				else
				{
					newTarget = Vector2(camera->GetCameraPos().x - (currentPos.x - targetPos.x - 30), camera->GetCameraPos().z);
				}
				PanTo(100, newTarget, dt);
			}
		}
		else
		{
			float dist = 0;
			if (!(Vector2(camera->GetCameraPos().x, camera->GetCameraPos().z) - targetPos).IsZero())
			{
				dist = (Vector2(camera->GetCameraPos().x, camera->GetCameraPos().z) - targetPos).Length();
			}
			PanTo(camera->f_OrthoSize * 2 + dist, targetPos, dt);
		}
	}
}

void MapController::PanTo(float speed, Vector2 pos, double dt)
{
	Vector3 targetPos(pos.x, 0, pos.y);
	Vector3 vel = (targetPos - camera->GetCameraPos()).Normalized() * speed * (float)dt;
	if ((camera->GetCameraPos() - targetPos).LengthSquared() < 3 * 3)
	{
		camera->SetCameraPos(Vector3(targetPos.x,camera->GetCameraPos().y,targetPos.z));
		camera->SetCameraTarget(Vector3(targetPos.x, camera->GetCameraTarget().y, targetPos.z));
	}
	else
	{
		camera->SetCameraPos(camera->GetCameraPos() + Vector3(vel.x, 0, vel.z));
		camera->SetCameraTarget(camera->GetCameraTarget() + Vector3(vel.x, 0, vel.z));
	}
}

void MapController::GetUnitPath()
{
	if (selectedUnit)
	{
		if (!map->GetCharacter(selectedTile.x, selectedTile.y) && !map->GetEnemy(selectedTile.x, selectedTile.y) && map->theScreenMap[selectedTile.y][selectedTile.x] == 0)
		{
			AStar search((int)selectedUnit->getPos().x, (int)selectedUnit->getPos().y, selectedTile.x, selectedTile.y, map);
			if ((selectedUnit->getPos() - Vector2(selectedTile.x, selectedTile.y)).Length() > selectedUnit->i_movementCost)
			{
				b_canPlace = false;
			}
			else if (search.Search())
			{
				if (search.bestPath.size() <= (selectedUnit->i_movementCost + 1))
				{
					int tempcost = 0;
					for (int i = 0; i < search.bestPath.size(); ++i)
					{
						if (i != 0)
							tempcost += map->GetObstacle(search.bestPath[i].x, search.bestPath[i].y).cost + 1;
					}
					if (tempcost <= selectedUnit->i_movementCost)
					{
						selectedUnit->i_stepsTaken = tempcost;
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
			b_forceCamera = true;
			if (!(unitPath[0] - selectedUnit->getPos()).IsZero())
			{
				Vector2 vel = (unitPath[0] - selectedUnit->getPos()).Normalized() * speed * dt;
				selectedUnit->setPos(selectedUnit->getPos() + vel);
				selectedTile = selectedUnit->getPos();
			}
			if ((unitPath[0] - selectedUnit->getPos()).Length() < 0.1f)
			{
				selectedUnit->setPos(unitPath[0]);
				selectedTile = selectedUnit->getPos();
				unitPath.erase(unitPath.begin());
			}
		}
		else
		{
			b_forceCamera = false;
			b_movingUnit = false;
			map->movePath.clear();
			OpenButtons();
			map->theScreenMap[(int)selectedUnit->getPos().y][(int)selectedUnit->getPos().x] = 2;
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
	atkRangeIndicator.clear();
	attackableUnits.clear();

	{
		const struct { int x, y; } succ[4] = { { 0, -1 }, { 0, 1 }, { 1, 0 }, { -1, 0 } };
		for (int i = 0; i < 4; ++i)
		{
			atkRangeIndicator.push_back(Vector2(selectedUnit->getPos().x + succ[i].x, selectedUnit->getPos().y + succ[i].y));
			if (map->GetEnemy(selectedUnit->getPos().x + succ[i].x, selectedUnit->getPos().y + succ[i].y))
			{
				attackableUnits.push_back(map->GetEnemy(selectedUnit->getPos().x + succ[i].x, selectedUnit->getPos().y + succ[i].y));
				b_activeButtons[ATTACK] = true;
			}
		}
	}
	if (selectedUnit->i_attackRange > 1)
	{
		const struct { int x, y; } succ[8] = { { -1, -1 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { -2, 0 }, { 2, 0 }, { 0, -2 }, { 0, 2 } };
		for (int i = 0; i < 8; ++i)
		{
			atkRangeIndicator.push_back(Vector2(selectedUnit->getPos().x + succ[i].x, selectedUnit->getPos().y + succ[i].y));
			if (map->GetEnemy(selectedUnit->getPos().x + succ[i].x, selectedUnit->getPos().y + succ[i].y))
			{
				attackableUnits.push_back(map->GetEnemy(selectedUnit->getPos().x + succ[i].x, selectedUnit->getPos().y + succ[i].y));
				b_activeButtons[ATTACK] = true;
			}
		}
	}

	if (selectedUnit->i_specialMeter >= 100 && b_activeButtons[ATTACK])
	{
		b_activeButtons[SPECIAL] = true;
	}

	if (selectedUnit->i_stepsTaken == 0)
	{
		b_activeButtons[MOVE] = true;
	}

	b_activeButtons[WAIT] = true;

	if (b_activeButtons[ATTACK])
	{
		currentButton = ATTACK;
	}
	else if (b_activeButtons[SPECIAL])
	{
		currentButton = SPECIAL;
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
	b_activeButtons[SPECIAL] = false;
	b_activeButtons[MOVE] = false;
	b_activeButtons[WAIT] = false;

	if (selectedButton == WAIT)
	{
		selectedUnit->b_tookAction = true;
		selectedUnit = nullptr;
		for (int i = 0; i < map->characters.size(); ++i)
		{
			if (map->characters[i]->b_tookAction == false)
			{
				selectedTile = map->characters[i]->getPos();
				break;
			}
		}
	}
	else if (selectedButton == ATTACK || selectedButton == SPECIAL)
	{
		b_forceCamera = true;
		b_attacking = true;
		selectedTile = attackableUnits[0]->getPos();
		selectedEnemy = attackableUnits[0];
	}
}

void MapController::ButtonControls()
{
	if (b_activeButtons[WAIT]) // If buttons are active
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
		{
			if (currentButton == ATTACK)
			{
				currentButton = WAIT;
			}
			else
			{
				currentButton = (CTRLBUTTONS)(currentButton - 1);
				while (!b_activeButtons[currentButton])
				{
					if (currentButton != ATTACK)
					{
						currentButton = (CTRLBUTTONS)(currentButton - 1);
					}
					else
					{
						currentButton = (CTRLBUTTONS)(CTRL_TOTAL - 1);
					}
				}
			}
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
		{
			if (currentButton == WAIT)
			{
				currentButton = ATTACK;
			}
			else
			{
				currentButton = (CTRLBUTTONS)(currentButton + 1);
			}
			while (!b_activeButtons[currentButton])
			{
				if (currentButton != WAIT)
				{
					currentButton = (CTRLBUTTONS)(currentButton + 1);
				}
				else
				{
					currentButton = (CTRLBUTTONS)(0);
				}
			}
		}
	}
}

void MapController::AttackControls()
{
	if (b_attacking) // If buttons are active
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
		{
			if (enemyIterator >= attackableUnits.size() - 1)
			{
				enemyIterator = 0;
			}
			else
			{
				enemyIterator++;
			}
			selectedTile = attackableUnits[enemyIterator]->getPos();
			selectedEnemy = attackableUnits[enemyIterator];
		}
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
		{
			if (enemyIterator == 0)
			{
				enemyIterator = attackableUnits.size() - 1;
			}
			else
			{
				enemyIterator--;
			}
			selectedTile = attackableUnits[enemyIterator]->getPos();
			selectedEnemy = attackableUnits[enemyIterator];
		}
	}
}

void MapController::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	// Rendering selected tile
	if (!b_movingUnit)
	{
		modelStack.PushMatrix();
		modelStack.Translate(selectedTile.x * map->tileSizeX + map->tileSizeX / 2, -1, selectedTile.y * map->tileSizeY + map->tileSizeY / 2);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(map->tileSizeX, map->tileSizeY, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("SelectedArrow"));
		modelStack.PopMatrix();
	}

	if (b_attacking)
	{
		for (int i = 0; i < atkRangeIndicator.size(); ++i)
		{
			if (atkRangeIndicator[i].x >= 0 && atkRangeIndicator[i].x < map->numTilesWidth && atkRangeIndicator[i].y >= 0 && atkRangeIndicator[i].y < map->numTilesHeight)
			{
				modelStack.PushMatrix();
				modelStack.Translate(atkRangeIndicator[i].x * map->tileSizeX + map->tileSizeX / 2, -0.2, atkRangeIndicator[i].y * map->tileSizeY + map->tileSizeY / 2);
				modelStack.Rotate(90, 1, 0, 0);
				modelStack.Scale(map->tileSizeX, map->tileSizeY, 1);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("RedSelected"));
				modelStack.PopMatrix();
			}
		}
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
			modelStack.Translate(20, f_yOffset, 1.f);
			f_yOffset -= 12.5f;
			modelStack.Scale(10 * 4 / 3, 10, 1.f);
			if ((int)currentButton == i)
			{
				modelStack.Scale(1.5f, 1.5f, 1.f);
			}
			if (i == ATTACK)
			{
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Attack"));
			}
			else if (i == SPECIAL)
			{
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Special"));
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

	if (selectedUnit && selectedButton == MOVE && !b_movingUnit)
	{
		string textdisplay;
		if (map->movePath.size() - 1 <= selectedUnit->i_movementCost && b_canPlace)
		{
			textdisplay = "Movement cost:" + std::to_string(selectedUnit->i_stepsTaken) + "/" + std::to_string(selectedUnit->i_movementCost);
		}
		else
		{
			textdisplay = "Invalid point";
		}

		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), textdisplay, Vector3(-30, 50, 1), 7.f, Color(1, 1, 1));
	}

	if (!b_movingUnit && *b_playerTurn)
	{
		if (selectedUnit)
		{
			string Name = "Name:" + selectedUnit->getName();
			string HP = "HP:" + std::to_string(selectedUnit->getCurrentHP()) + "/" + std::to_string(selectedUnit->getMaxHP());
			string DMG = "DMG:" + std::to_string(selectedUnit->getDamage());
			string SPL = "Special:" + std::to_string(selectedUnit->i_specialMeter) + "/100";

			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), Name, Vector3(-50, 50, 1), 7.f, Color(0.3, 0.3, 1));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), HP, Vector3(-50, 40, 1), 5.f, Color(0.3, 0.3, 1));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), DMG, Vector3(-50, 30, 1), 5.f, Color(0.3, 0.3, 1));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), SPL, Vector3(-50, 20, 1), 5.f, Color(0.3, 0.3, 1));
			RenderHelper::Render2DMesh(MeshBuilder::GetInstance()->GetMesh("Frame"), Vector3(-47, 35, 0.9), Vector3(60, 60, 50), Vector3(0, 0, 0));
			RenderHelper::Render2DMesh(selectedUnit->getPortrait(), Vector3(-62, 40, 1), Vector3(15, 15 * 16 / 9, 20), Vector3(0, 0, 0));
		}
		else if (map->GetCharacter(selectedTile.x, selectedTile.y))
		{
			Character* temp = map->GetCharacter(selectedTile.x, selectedTile.y);
			string Name = "Name:" + temp->getName();
			string HP = "HP:" + std::to_string(temp->getCurrentHP()) + "/" + std::to_string(temp->getMaxHP());
			string DMG = "DMG:" + std::to_string(temp->getDamage());
			string SPL = "Special:" + std::to_string(temp->i_specialMeter) + "/100";

			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), Name, Vector3(-50, 50, 1), 7.f, Color(0.3, 0.3, 1));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), HP, Vector3(-50, 40, 1), 5.f, Color(0.3, 0.3, 1));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), DMG, Vector3(-50, 30, 1), 5.f, Color(0.3, 0.3, 1));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), SPL, Vector3(-50, 20, 1), 5.f, Color(0.3, 0.3, 1));
			RenderHelper::Render2DMesh(MeshBuilder::GetInstance()->GetMesh("Frame"), Vector3(-47, 35, 0.9), Vector3(60, 60, 50), Vector3(0, 0, 0));
			RenderHelper::Render2DMesh(temp->getPortrait(), Vector3(-62, 40, 1), Vector3(15, 15 * 16/9, 20), Vector3(0, 0, 0));
		}

		if (map->GetEnemy(selectedTile.x, selectedTile.y))
		{
			Character* temp = map->GetEnemy(selectedTile.x, selectedTile.y);
			string Name = "Name:" + temp->getName();
			string HP = "HP:" + std::to_string(temp->getCurrentHP()) + "/" + std::to_string(temp->getMaxHP());
			string DMG = "DMG:" + std::to_string(temp->getDamage());

			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), Name, Vector3(50, 50, 1), 7.f, Color(1, 0.3, 0.3));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), HP, Vector3(50, 40, 1), 5.f, Color(1, 0.3, 0.3));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), DMG, Vector3(50, 30, 1), 5.f, Color(1, 0.3, 0.3));
			RenderHelper::Render2DMesh(MeshBuilder::GetInstance()->GetMesh("Frame"), Vector3(65, 35, 0.9), Vector3(60, 60, 50), Vector3(0, 0, 0));
			RenderHelper::Render2DMesh(temp->getPortrait(), Vector3(75, 40, 1), Vector3(15, 15 * 16 / 9, 20), Vector3(0, 0, 0));
		}
	}
}