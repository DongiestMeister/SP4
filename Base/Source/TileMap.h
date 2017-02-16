#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <string>
#include "Vector3.h"
#include "PlayerInfo.h"

using std::vector;
using std::string;


class TileMap
{
public:
	TileMap();
	~TileMap();
	
	void Init(int screenHeight, int screenWidth, int numTilesHeight, int numTilesWidth);

	int screenHeight, screenWidth, numTilesHeight, numTilesWidth;
	float tileSizeX, tileSizeY;
	vector<vector<int> > theScreenMap;

	CharactersList characters;
	CharactersList enemies;
	
	vector<Vector2> movePath;

	bool LoadMap(const string mapName);
	void Render();

	void AddCharacter(int x, int y, Character* unit);
	Character *GetCharacter(int x, int y);

	void AddEnemy(int x, int y, Character* unit);
	Character *GetEnemy(int x, int y);

	void ResetCharacters();
	void ResetEnemies();
private:
	void ClearCharacters();
};


#endif