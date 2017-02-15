#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include "SingletonTemplate.h"
#include "Character\Character.h"

#include <string>

using std::string;

class PlayerInfo : public Singleton<PlayerInfo>
{
	friend Singleton<PlayerInfo>;

public:
	PlayerInfo() { player = NULL; enemy = NULL; }
	~PlayerInfo() {}

	Character *player;
	Character *enemy;

	bool b_attacking = true;
	


protected:



private:
	


};



#endif