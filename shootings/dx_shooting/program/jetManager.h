#pragma once
#include "vec3.h"
class Tama;
class Jet;
class PlayerJet;
class EnemyJet;
class Animation;
//グラフィックの管理もここで行う。
class JetManager {
public:
	static JetManager* getInstance();
	enum teisu {MAX_ANIM_SUU=30,MAX_ENEMY_SUU = 10,MAX_SHOT_SUU = 100,MAX_TARGET_SUU=100,BOMB_ANIM_SUU=12,MAX_GFX_SUU=20,MAX_ARM_GFX_SUU=3,MAX_SHOT_GFX_SUU=10,MAX_UI=8};//最大数などの定数を入れる
	enum jetgfx{NONE,ZIKI_ZENTAI,ZIKI_HOUDAI,ZIKI_JET,ENEMY_ZENTAI,ENEMY_HOUDAI,ENEMY_JET_TYPE1,BOOST};//jetを構成するグラフィックス
	enum shotgfx{FIRE1,MISS1,MISS2,BEAM1,BEAM2,BOOMERANG};//弾のグラフィクス
	enum weapongfx{NORMAL_WEAPON,FIRE_WEAPON,BEAM_WEAPON};//jetの武器のグラフィクス
	enum uigfx{MIGI,HIDARI};
	//enum shottype {SHOT_FIRE1,SHOT_BEAM1,SHOT_MISSILE1,SHOT_MISSILE2,};
	int gfx[MAX_GFX_SUU];
	int shotGfx[MAX_SHOT_GFX_SUU];
	int armgfx[MAX_ARM_GFX_SUU];
	int bomAnim[BOMB_ANIM_SUU];
	int UIgfx[MAX_UI];
	JetManager();
	Animation* anims[MAX_ANIM_SUU];
	PlayerJet* player;
	EnemyJet* enemy[MAX_ENEMY_SUU];
	Jet* targetJet[MAX_TARGET_SUU];
	bool hitHantei(Tama* a, Tama* b);
	bool mainHaveWeapons[10];
	bool subHaveWeaponSlot[10];
	void shotIconDraw(int fx, int fy,int atype,int type,int fsize);
	void init();
	void animStart(t2k::vec3,int Img[]);
	void animationUpdate();
	void inToTarget(Jet* target);
	void clearTarget();
	void damagesSyori(Jet* shooter);
	int getGraphYsize(int img);
};