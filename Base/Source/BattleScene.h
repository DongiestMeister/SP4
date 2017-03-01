#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include "Scene.h"
#include "Mtx44.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include <vector>
#include "Sounds/Music.h"
#include "Lua\LuaInterface.h"
#include "Character/MeleeCharacter.h"
#include "Character/Weapon.h"
#include "PlayerInfo.h"
#include "DamageText.h"
#include "Character\Character.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;

using std::vector;

class BattleScene : public Scene
{
public:
	BattleScene();
	~BattleScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void LoadMeshes();
	void RunBattleAnimation(double dt, bool ranged, int dmgvalue);
	void RenderProps();
	void RenderUnitsModels(MS& _ms);
	void TakenHitAnimation(double dt);	//shake it shake it
	void RenderSkyBox();
	void LightMouseControl(double dt);
	void CameraClash(bool is_player, bool ranged, double dt);
	void CameraClashReturn(double dt);
	void PlayerAnimation(double dt, bool ranged, int dmgvalue, float maxdist);
	void EnemyAnimation(double dt, bool ranged , int dmgvalue, float maxdist);

	/*RenderTextStuff
	- runs animation and appearance of Totaldamage Counter and Effects
	- Removes completed damage text through list*/
	void RenderTextStuff(double dt, int dmgvalue);

	void SetBattleBonus(bool _isBonus) { b_bonusRush = _isBonus; }
	bool GetBattleBonus() { return b_bonusRush; }


private:
	ShaderProgram* currProg;
	//CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	//TextEntity* textObj[6];
	Light* lights[2];

	Character *player;
	Character *enemy;

	float player_posx;
	float enemy_posx;
	float f_textDelayOnScreen;
	float f_SceneIntroDelay;
	float fps;
	float f_bonus_time;
	float f_shakedelay;
	//float f_shakedelayReturn;

	bool b_bonus_start;
	bool b_isClashed;
	bool b_bonusRush;
	bool b_spamLock;

	bool b_shaking;
	bool shake_dir;
	bool shoot_once;
	bool shoot_hit;


	vector<DamageText*> storeDmgTxt;
	vector<float> f_projectiles;
	vector<float> f_projectiles_enemy;
	TextEntity* TotalDamage;
	TextEntity* TotalDmgCheer;
	TextEntity* TimerText;

	int i_totaldmg_txt;
	int i_shakecounter;





	ISound* BGM;
};

#endif