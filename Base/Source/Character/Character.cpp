#include "Character.h"
#include "LoadTGA.h"

Character::Character() : i_strBoostFromTerrain(0), i_dexBoostFromTerrain(0), i_lukBoostFromTerrain(0), i_Damage(0)
{
	strategy = OFFENCE;

	i_baseHP = 50;
	i_currentHP = 50;
	i_HP = 50;
	i_idInParty = -1;
	i_specialMeter = 0;

	i_STR = 0;
	i_DEX = 0;
	i_LUK = 0;
	character3DMesh = "";
	character2DMesh = "";
	characterPortrait = "";
	weapon = nullptr;
	armor = nullptr;

	b_tookAction = false;
	i_stepsTaken = 0;
	b_inParty = false;
	s_Name = "";

	FSM = nullptr;
	animation = nullptr;

	i_frames = 0;
}

Character::~Character()
{
	if (weapon)
	{
		//delete weapon;
		//weapon = nullptr;
	}
	if (armor)
	{
		//delete armor;
		//armor = nullptr;
	}
	if (FSM)
	{
		delete FSM;
		FSM = nullptr;
	}

	if (animation)
	{
		delete animation;
		animation = nullptr;
	}
}

int Character::getSTR()
{
	return i_STR;
}

int Character::getDEX()
{
	return i_DEX;
}

int Character::getLUK()
{
	return i_LUK;
}

int Character::getDamage()
{
	return i_Damage;
}

int Character::getCurrentHP()
{
	return i_currentHP;
}

int Character::getMaxHP()
{
	return i_HP;
}
Vector2 Character::getPos()
{
	return pos;
}

Mesh* Character::get3DMesh()
{
	return MeshBuilder::GetInstance()->GetMesh(character3DMesh);
}

Mesh* Character::get2DMesh()
{
	return MeshBuilder::GetInstance()->GetMesh(character2DMesh);
}

Mesh* Character::getPortrait()
{
	return MeshBuilder::GetInstance()->GetMesh(characterPortrait);
}

Weapon* Character::getWeapon()
{
	if (weapon)
		return weapon;
	return nullptr;
}

Armor* Character::getArmor()
{
	if (armor)
		return armor;
	return nullptr;
}

string Character::getName()
{
	return s_Name;
}

void Character::setSTR(int newSTR)
{
	i_STR = newSTR;
}

void Character::setDEX(int newDEX)
{
	i_DEX = newDEX;
}

void Character::setLUK(int newLUK)
{
	i_LUK = newLUK;
}

void Character::setBaseSTR(int newSTR)
{
	i_baseSTR = newSTR;
}

void Character::setBaseDEX(int newDEX)
{
	i_baseDEX = newDEX;
}

void Character::setBaseLUK(int newLUK)
{
	i_baseLUK = newLUK;
}
void Character::setDamage(int newDamage)
{
	i_Damage = newDamage;
}

void Character::setHP(int newHP)
{
	i_baseHP = newHP;
}

void Character::setPos(Vector2 newPos)
{
	pos = newPos;
}

void Character::set3DMesh(string meshName)
{
	character3DMesh = meshName;
}

void Character::set2DMesh(string meshName)
{
	character2DMesh = meshName;
}

void Character::setAnimation(string filename,int frames)
{
	animation = MeshBuilder::GetInstance()->GenerateSpriteAnimation("Animation", 1, frames, 1);
	animation->textureID = LoadTGA(filename.c_str());

	if (animation)
	{
		animation->m_anim = new Animation();
		animation->m_anim->Set(0, frames - 1, 1, 1.5f, true);
	}
}

void Character::setPortrait(string meshName)
{
	characterPortrait = meshName;
}

void Character::setName(string newName)
{
	s_Name = newName;
}
void Character::takeDamage(int dmg)
{
	i_currentHP -= dmg;
}

void Character::calculateStats()
{
	if (armor)
	{
		i_HP = armor->i_hpBoost + i_baseHP;
		i_currentHP = i_HP;
		i_STR = armor->i_strBoost + i_strBoostFromTerrain + i_baseSTR;
		i_DEX = armor->i_dexBoost + i_dexBoostFromTerrain + i_baseDEX;
		i_LUK = armor->i_lukBoost + i_lukBoostFromTerrain + i_baseLUK;
	}
	else
	{
		i_HP = i_baseHP;
		i_currentHP = i_HP;
		i_STR = i_strBoostFromTerrain + i_baseSTR;
		i_DEX = i_dexBoostFromTerrain + i_baseDEX;
		i_LUK = i_lukBoostFromTerrain + i_baseLUK;
	}
	if (weapon)
	{
		i_Damage = ((0.2 * i_STR) + (0.3 * (weapon->i_damageValue))) + 2;
	}
	else
	{
		i_Damage = (0.2 * i_STR) + 25;
	}
}

void Character::equipWeapon(Weapon* newWeapon)
{
	if (newWeapon == nullptr || weapon == newWeapon)
		return;
	if (newWeapon->b_isEquippedToSomeone == false)
	{
		if (weapon)
		{
			weapon->s_ownerName = "";
			weapon->b_isEquippedToSomeone = false;
		}
		weapon = newWeapon;
		weapon->s_ownerName = s_Name;
		weapon->b_isEquippedToSomeone = true;
		calculateStats();
	}
}

void Character::equipArmor(Armor* newArmor)
{
	if (newArmor == nullptr || armor == newArmor)
		return;
	if (newArmor->b_isEquippedToSomeone == false)
	{
		if (armor)
		{
			armor->s_ownerName = "";
			armor->b_isEquippedToSomeone = false;
		}
		armor = newArmor;
		armor->s_ownerName = s_Name;
		armor->b_isEquippedToSomeone = true;
		calculateStats();
	}
}