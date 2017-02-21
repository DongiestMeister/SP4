#include "Armor.h"

Armor::Armor()
{
	s_Name = "";
	b_isEquippedToSomeone = false;
	b_isArmor = true;
	b_isWeapon = false;
	i_strBoost = -1;
	i_dexBoost = -1;
	i_lukBoost = -1;
	i_hpBoost = -1;
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