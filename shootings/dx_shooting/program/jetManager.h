#pragma once
#include "vec3.h"
class PlayerJet;
class EnemyJet;
class Jet;
class Animation;
class Tama;
class JetManager {
public:
	static JetManager* getInstance();
	enum teisu {MAX_ANIM_SUU=30,MAX_ENEMY_SUU = 10,MAX_SHOT_SUU = 100,MAX_TARGET_SUU=100,BOMB_ANIM_SUU=12,MAX_GFX_SUU=20};
	enum gfxtype{NONE,FIRE,ZIKI_ZENTAI,ZIKI_HOUDAI,ZIKI_JET,ZIKI_MISS,ZIKI_BEAM,ENEMY_ZENTAI,ENEMY_HOUDAI,ENEMY_JET_TYPE1,ENEMY_MISS,ENEMY_BEAM};
	enum shottype {SHOT_FIRE1,SHOT_BEAM1,SHOT_MISSILE1,SHOT_MISSILE2,};
	int gfx[MAX_GFX_SUU];
	int bomAnim[BOMB_ANIM_SUU];
	JetManager();
	Animation *anims[MAX_ANIM_SUU];
	PlayerJet* player;
	EnemyJet* enemy[MAX_ENEMY_SUU];
	Jet* targetJet[MAX_TARGET_SUU];
	bool hitHantei(Tama* a, Tama* b);

	void animStart(t2k::vec3,int Img[]);
	void animationUpdate();
	void inToTarget(Jet* target);
	void clearTarget();
	void damagesSyori(Jet* shooter);
	int getGraphYsize(int img);
};