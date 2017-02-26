#ifndef ENEMYSPAWNER_H
#define ENEMYSPAWNER_H

#include <vector>
#include <string>
#include "Vector3.h"
#include "../TileMap.h"

using std::vector;
using std::string;

class EnemySpawner
{
public:
	EnemySpawner();
	~EnemySpawner();

	void Init(TileMap *map);


private:

};


#endif