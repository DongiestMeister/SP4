#include "Armor.h"

Armor::Armor() : i_strBoost(0), i_dexBoost(0), i_lukBoost(0), i_hpBoost(0)
{
}

Armor::Armor(int str, int dex, int luk, int hp, bool equipped)
{
	i_strBoost = str;
	i_dexBoost = dex;
	i_lukBoost = luk;
	i_hpBoost = hp;
	b_isEquippedToSomeone = equipped;
}

Armor::~Armor()
{
}