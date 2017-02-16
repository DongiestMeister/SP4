#include "Armor.h"

Armor::Armor() : i_strBoost(0), i_dexBoost(0), i_lukBoost(0), i_hpBoost(0), s_Name("")
{
}

Armor::Armor(int str, int dex, int luk, int hp, bool equipped, string name)
{
	i_strBoost = str;
	i_dexBoost = dex;
	i_lukBoost = luk;
	i_hpBoost = hp;
	b_isEquippedToSomeone = equipped;
	s_Name = name;
}

Armor::~Armor()
{
}