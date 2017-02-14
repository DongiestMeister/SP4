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

	for (vector<Vector3>::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Vector3 character = (*it);
		
		modelStack.PushMatrix();
		modelStack.Translate(character.x * tileSizeX + tileSizeX / 2, -0.2, character.z * tileSizeY + tileSizeY / 2);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(tileSizeX, tileSizeY, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Knight"));
		modelStack.PopMatrix();
	}
}

void TileMap::AddCharacter(int x, int y)
{
	characters.push_back(Vector3(x, 0, y));
}

Vector3 TileMap::GetCharacter(int x, int y)
{
	for (vector<Vector3>::iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Vector3 character = *it;
		if ((int)character.x == x && (int)character.z == y)
		{
			return character;
		}
	}

	return Vector3(-1, -1, -1);
}