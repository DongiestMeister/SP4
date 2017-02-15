#include "TileMap.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"


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
}

bool TileMap::LoadMap(const string mapName)
{
	int theLineCounter = numTilesHeight - 1;
	int theMaxNumOfColumns = 0;

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
				theScreenMap[theLineCounter][theColumnCounter++] = atoi(token.c_str()); // theScreenMap[y][x]
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
			if (theScreenMap[j][i] == 0)
			{
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN"));
			}
			modelStack.PopMatrix();
		}
	}

	for (vector<Unit*>::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Unit *character = (*it);
		
		modelStack.PushMatrix();
		modelStack.Translate(character->pos.x * tileSizeX + tileSizeX / 2, -0.4, character->pos.y * tileSizeY + tileSizeY / 2);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(tileSizeX, tileSizeY, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Knight"));
		modelStack.PopMatrix();
	}

	if (movePath.size() > 0)
	{
		for (int i = 0; i < movePath.size(); ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(movePath[i].x * tileSizeX + tileSizeX / 2, -0.2, movePath[i].y * tileSizeY + tileSizeY / 2);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(tileSizeX, tileSizeY, 1);
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Selected"));
			modelStack.PopMatrix();
		}
	}
}

void TileMap::AddCharacter(int x, int y,Unit *unit)
{
	unit->pos.Set(x, y);
	characters.push_back(unit);
}

Unit* TileMap::GetCharacter(int x, int y)
{
	for (vector<Unit*>::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Unit *character = *it;
		if ((int)character->pos.x == x && (int)character->pos.y == y)
		{
			return character;
		}
	}

	return nullptr;
}

void TileMap::ClearCharacters()
{
	for (vector<Unit*>::iterator it = characters.begin(); it != characters.end();)
	{
		delete *it;
		it = characters.erase(it);
	}

	for (vector<Unit*>::iterator it = enemies.begin(); it != enemies.end();)
	{
		delete *it;
		it = characters.erase(it);
	}
}