#include "TileMap.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "AI\AI_DefenceFSM.h"

#include "PlayerInfo.h"

#include "AI\AI_OffenceFSM.h"

using namespace std;

TileMap::TileMap()
{

}

TileMap::~TileMap()
{
	ClearCharacters();
}

void TileMap::Init(int screenHeight, int screenWidth, int numTilesHeight, int numTilesWidth)
{
	this->screenHeight = screenHeight;
	this->screenWidth = screenWidth;
	this->numTilesHeight = numTilesHeight;
	this->numTilesWidth = numTilesWidth;
	this->tileSizeX = (float)screenWidth / (float)numTilesWidth;
	this->tileSizeY = (float)screenHeight / (float)numTilesHeight;

	theScreenMap.resize(numTilesHeight);
	for (int i = 0; i < numTilesHeight; ++i)
		theScreenMap[i].resize(numTilesWidth);

	for (int i = 0; i < numTilesHeight; ++i)
	{
		for (int j = 0; j < numTilesWidth; ++j)
		{
			theScreenMap[i][j] = 0;
		}
	}

	for (int i = 0; i < PlayerInfo::GetInstance()->party.size(); ++i)
	{
		characters.push_back(PlayerInfo::GetInstance()->party[i]->clone());
	}

	for (int i = 0; i < characters.size(); ++i)
	{
		characters[i]->setPos(Vector2(i, 0));
		theScreenMap[0][i] = 2;
	}

	cout << enemies.size() << endl;
	for (int i = 0; i < enemies.size(); ++i)
	{
		cout << enemies[i]->getPos() << endl;
		theScreenMap[(int)enemies[i]->getPos().y][(int)enemies[i]->getPos().x] = 2;
	}
}

bool TileMap::LoadMap(const string mapName)
{
	int theLineCounter = numTilesHeight - 1;
	int theMaxNumOfColumns = 0;
	enemySpawnPoints.clear();

	ifstream file(mapName.c_str());
	if (file.is_open())
	{
		int i = 0, k = 0;
		while (file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			if (theLineCounter < 0)
				break;

			int theColumnCounter = 0;

			string token;
			istringstream iss(aLineOfText);
			while (getline(iss, token, ',') && (theColumnCounter<numTilesWidth))
			{
				if (atoi(token.c_str()) == 3) // Forest tile
				{
					AddObstacle(theColumnCounter, theLineCounter, 3, 1);
					theScreenMap[theLineCounter][theColumnCounter++] = 0;
				}
				else if (atoi(token.c_str()) == 4) // Ocean tile
				{
					AddObstacle(theColumnCounter, theLineCounter, 4, 2);
					theScreenMap[theLineCounter][theColumnCounter++] = 0;
				}
				else if (atoi(token.c_str()) == 5) // Goal tile
				{
					AddObstacle(theColumnCounter, theLineCounter, 5, 0);
					theScreenMap[theLineCounter][theColumnCounter++] = 0;		
				}
				else if (atoi(token.c_str()) == 10)
				{
					enemySpawnPoints.push_back(Vector2(theColumnCounter, theLineCounter));
					theScreenMap[theLineCounter][theColumnCounter++] = 0;
				}
				else if (theScreenMap[theLineCounter][theColumnCounter] != 2)
				{
					theScreenMap[theLineCounter][theColumnCounter++] = atoi(token.c_str()); // theScreenMap[y][x]
				}
				else
				{
					theColumnCounter++;
				}
			}
			theLineCounter--;
		}
	}

	file.close();
	return true;
}

void TileMap::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	for (int i = 0; i < numTilesWidth; ++i)
	{
		for (int j = 0; j < numTilesHeight; ++j)
		{
			modelStack.PushMatrix();
			modelStack.Translate(i * tileSizeX + tileSizeX / 2, -0.1, j * tileSizeY + tileSizeY / 2);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(tileSizeX, tileSizeY, 1);
			if (theScreenMap[j][i] == 0 || theScreenMap[j][i] == 2 || theScreenMap[j][i] == 1)
			{
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN"));
			}
			modelStack.PopMatrix();

			if (theScreenMap[j][i] == 1)
			{
				modelStack.PushMatrix();
				modelStack.Translate(i * tileSizeX + tileSizeX / 2, -0.11, j * tileSizeY + tileSizeY / 2);
				modelStack.Rotate(90, 1, 0, 0);
				modelStack.Scale(tileSizeX, tileSizeY, 1);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Boulder"));
				modelStack.PopMatrix();
			}
		}
	}

	for (CharactersList::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Character *unit = (*it);
		
		modelStack.PushMatrix();
		modelStack.Translate(unit->getPos().x * tileSizeX + tileSizeX / 2, -0.4, unit->getPos().y * tileSizeY + tileSizeY / 2);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(tileSizeX, tileSizeY, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("BlueKnight"));
		modelStack.PopMatrix();
	}

	for (vector<Character*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
	{
		Character *unit = (*it);

		modelStack.PushMatrix();
		modelStack.Translate(unit->getPos().x * tileSizeX + tileSizeX / 2, -0.4, unit->getPos().y * tileSizeY + tileSizeY / 2);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(tileSizeX, tileSizeY, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Knight"));
		modelStack.PopMatrix();
	}

	for (int i = 0; i < obstacleList.size(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(obstacleList[i].pos.x * tileSizeX + tileSizeX / 2, -0.3, obstacleList[i].pos.y * tileSizeY + tileSizeY / 2);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(tileSizeX, tileSizeY, 1);
		if (obstacleList[i].type == 3)
		{
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Forest"));
		}
		else if (obstacleList[i].type == 4)
		{
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Ocean"));
		}
		else if (obstacleList[i].type == 5)
		{
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Flag"));
		}
		modelStack.PopMatrix();
	}

	if (movePath.size() > 0)
	{
		for (int i = 0; i < movePath.size(); ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(movePath[i].x * tileSizeX + tileSizeX / 2, -0.35, movePath[i].y * tileSizeY + tileSizeY / 2);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(tileSizeX, tileSizeY, 1);
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Selected"));
			modelStack.PopMatrix();
		}
	}
}

void TileMap::AddCharacter(int x, int y, Character* unit)
{
	unit->setPos(Vector2(x, y));
	characters.push_back(unit);
}

Character* TileMap::GetCharacter(int x, int y)
{
	for (CharactersList::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Character *unit = *it;
		if ((int)unit->getPos().x == x && (int)unit->getPos().y == y)
		{
			return unit;
		}
	}

	return nullptr;
}

void TileMap::AddEnemy(int x, int y, Character *unit)
{
	unit->setPos(Vector2(x, y));
	theScreenMap[y][x] = 2;
	if (unit->strategy == Character::DEFENCE)
	{
		unit->FSM = new AI_DefenceFSM(unit, AI_DefenceFSM::IDLE);
		unit->FSM->map = this;
	}
	if (unit->strategy == Character::NEUTRAL)
	{
		unit->FSM = new AI_DefenceFSM(unit, AI_DefenceFSM::STATIONARY);
		unit->FSM->map = this;
	}
	else if (unit->strategy == Character::OFFENCE)
	{
		unit->FSM = new AI_OffenceFSM(unit);
		unit->FSM->map = this;
	}
	enemies.push_back(unit);
}

Character* TileMap::GetEnemy(int x, int y)
{
	for (vector<Character*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
	{
		Character *unit = *it;
		if ((int)unit->getPos().x == x && (int)unit->getPos().y == y)
		{
			return unit;
		}
	}

	return nullptr;
}

void TileMap::ClearCharacters()
{
	for (CharactersList::iterator it = characters.begin(); it != characters.end();)
	{
		delete *it;
		it = characters.erase(it);
	}

	for (CharactersList::iterator it = enemies.begin(); it != enemies.end();)
	{
		delete *it;
		it = enemies.erase(it);
	}
}

void TileMap::ResetCharacters()
{
	for (int i = 0; i < characters.size(); ++i)
	{
		characters[i]->b_tookAction = false;
		characters[i]->i_stepsTaken = 0;
	}
}

void TileMap::ResetEnemies()
{
	for (int i = 0; i < enemies.size(); ++i)
	{
		enemies[i]->b_tookAction = false;
		enemies[i]->i_stepsTaken = 0;
	}
}

void TileMap::AddObstacle(int x, int y, int type, int cost)
{
	Obstacle temp(type, cost);
	temp.pos = Vector2(x, y);
	obstacleList.push_back(temp);
}

Obstacle TileMap::GetObstacle(int x, int y)
{
	for (int i = 0; i < obstacleList.size(); ++i)
	{
		if ((int)obstacleList[i].pos.x == x && (int)obstacleList[i].pos.y == y)
		{
			return obstacleList[i];
		}
	}

	return Obstacle(x, y, 0, 0);
}

void TileMap::RemovePlayer(Character* character)
{
	for (vector<Character*>::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Character* temp = *it;
		if (temp == character)
		{
			delete *it;
			characters.erase(it);
			break;
		}
	}
}

void TileMap::RemoveEnemy(Character* character)
{
	for (vector<Character*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
	{
		Character* temp = *it;
		if (temp == character)
		{
			theScreenMap[temp->getPos().y][temp->getPos().x] = 0;
			delete *it;
			enemies.erase(it);
			break;
		}
	}
}