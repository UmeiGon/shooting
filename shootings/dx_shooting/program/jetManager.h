#pragma once
#include "vec3.h"
class Tama;
class Jet;
class PlayerJet;
class EnemyJet;
class Animation;
class Item;
//グラフィックの管理もここで行う。
class JetManager {
public:
	static JetManager* getInstance();
	enum animsuu{ZANGEKI_ANIM_SUU=9,BOMB_ANIM_SUU=12,RAZER_ANIM_SUU=7,HOLE_ANIM_SUU=15,ULTBOMB1_ANIM_SUU=8,ULTBOMB2_ANIM_SUU=9};
	enum teisu {MAX_ANIM_SUU=50,MAX_ENEMY_SUU = 10,MAX_SHOT_SUU = 100,MAX_TARGET_SUU=100,MAX_GFX_SUU=20,MAX_ARM_GFX_SUU=3,MAX_SHOT_GFX_SUU=10,MAX_UI=8,MAX_ITEM_GFX=6,MAX_ITEM_SUU=100};//最大数などの定数を入れる
	enum jetgfx{NONE,ZIKI_ZENTAI,ZIKI_HOUDAI,ZIKI_JET,ENEMY_ZENTAI,ENEMY_HOUDAI,ENEMY_JET_TYPE1, ENEMY_JET_TYPE2, ENEMY_JET_TYPE3, ENEMY_JET_TYPE4, ENEMY_JET_TYPE5,BOOST,TARGET,};//jetを構成するグラフィックス
	enum shotgfx{FIRE1,MISS1,MISS2,BEAM1,BEAM2,BOOMERANG,BOMB_ICON,HOLE_ICON,RAZER_ICON,KOKI,KI,RICE};//弾のグラフィクス;
	enum weapongfx{NORMAL_WEAPON,FIRE_WEAPON,BEAM_WEAPON};//jetの武器のグラフィクス
	enum itemgfx{IRON,ENERGY,HOUNETU_GFX,CD_GFX,SPD_GFX,ATK_GFX};
	enum runetype{HOUNETU_RUNE,CD_RUNE,SPD_RUNE,ATK_RUNE};
	//enum shottype {SHOT_FIRE1,SHOT_BEAM1,SHOT_MISSILE1,SHOT_MISSILE2,};
	int gfx[MAX_GFX_SUU];
	int shotGfx[MAX_SHOT_GFX_SUU];
	int armgfx[MAX_ARM_GFX_SUU];
	int bomAnim[BOMB_ANIM_SUU];
	int zangekiAnim[ZANGEKI_ANIM_SUU];
	int razerAnim[RAZER_ANIM_SUU];
	int ultbom1Anim[ULTBOMB1_ANIM_SUU];
	int ultbom2Anim[ULTBOMB2_ANIM_SUU];
	int holeAnim[HOLE_ANIM_SUU];
	int itemGfx[MAX_ITEM_GFX];
	JetManager();
	Animation* anims[MAX_ANIM_SUU];
	PlayerJet* player;
	EnemyJet* enemy[MAX_ENEMY_SUU];
	Tama* targetJet[MAX_TARGET_SUU];
	Item* item[MAX_ITEM_SUU];
	t2k::vec3 ultpos;
	int ultActive;
	bool hitHantei(Tama* a, Tama* b);
	int enemySousa();
	void genStage1();
	void genStage2();
	void genStage3();
	void genStage0();
	EnemyJet* genEnemy(int ty,float times);
	void UltimateUpdate();
	void Ultimate(t2k::vec3 pos,int num);
	void JetManager::hitedEffect(Tama *hit);
	void shotIconDraw(int fx, int fy,int atype,int type,int fsize);
	void init();
	void animStart(t2k::vec3,int Img[],float Size=1.0f);
	void animationUpdate();
	void inToTarget(Tama* target);
	void clearTarget();
	void damagesSyori(Jet* shooter);
	int getGraphYsize(int img);
};