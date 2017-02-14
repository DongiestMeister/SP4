#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <string>
#include "Vector3.h"

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

	bool LoadMap(const string mapName);
	void Render();
};


#endif