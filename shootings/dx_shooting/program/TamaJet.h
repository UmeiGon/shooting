#pragma once
#include "jetManager.h"
class GameManager;
class Capsule;
typedef struct {
	int x;
	int y;
	float angle;
	int img;
	float gSize;
	float gAngle = 0;
}houData;
class Maru {
public:
	t2k::vec3 pos;
	int r;
	Maru(t2k::vec3 Pos, int R);
	Maru();
};
class Tama {
public:
	Capsule *capsule;
	GameManager *gm;
	float graphAngle;
	t2k::vec3 startPos;
	int Attack = 1;
	float liveTimer;
	float deathTime;
	float speed;
	Maru circle;
	int handle;
	float angle;
	float size = 1;
	float graphSize;
	Tama();
	Tama(float X, float Y, float Angle, float Size, float Spd, int atk, int gfx,float dtimer=-1);
	static bool maruHantei(Maru a, Maru b);
	static void LINE(t2k::vec3 a, t2k::vec3 b, int col);
	bool screenInside(float px, float py, float sabun);
	
	void gAngleTyosei(float ang);
	void addCapsuleHantei(int nagasa);
	void capTuizyu(t2k::vec3 Move, bool spined);
	virtual ~Tama();

	void draw();
};



class shotd {
public:
	shotd(float Siz, float Spd, int Atk, float Cd, float Heat, int Gfx, float dt = -1);
	float size;
	float speed;
	int atk;
	float shotCd;
	float shotHeat;
	int gfx;
	float deathTime;
};
class Jet :public Tama {
public:
	float atkTimer = 0;
	int health = 1;
	int maxhealth = 1;
	int handle2;
	float AttackSpeed = 1;
	enum StandardShotTypes { FIRE, BEAM, MISSILE1, MISSILE2 };
	houData *houdai;
	//生きていたらlive、スポーン前はtaiki、ライフ0で弾が画面にある場合dead、その後弾が無くなったらsyoukyo
	enum zyoutai { TAIKI, LIVE, DEAD, SYOUKYO };
	zyoutai stat = TAIKI;
	Tama *Shot[JetManager::MAX_SHOT_SUU];
	Jet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx,float dt);
	t2k::vec3 shotMoveTokusyu(int sn);
	void drawJet();
	void shotGen(shotd s, bool houdaiShot = false, int targetX=-1, int targetY=-1);
	void shotGen(StandardShotTypes s, bool houd = false, int targetX = -1, int targetY = -1);
	void drawHp(float startX, float startY, int Hp, int maxHp);
	void drawMoveShot(int i);
	void addHoudai(int plusX, int plusY, float ang, int img, float hsize, float gAng);
	~Jet();
protected:
	void shotGenHontai(float siz,float speed,int atk,int gfx,bool hou, int tx,int ty,float dt=-1);
};

class EnemyJet :public Jet {
public:
	float x0;
	float y0;
	float spawnTimer = 0;
	enum movetypes { MAWARU, NANAME, NAMI };
	movetypes moveType = MAWARU;
	EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx, float sT = 0, float dT = 20);
	void eneMove();

};

class PlayerJet :public Jet {
public:
	float heat;
	float maxHeat;
	float hounetu;
	float oneSecTimer;
	float overHeatTime;
	float subAtkTimer;
	bool moved;
	enum armtype{MAIN,SUB,ULT};
	enum teisu { OVERHEAT_SEC = 3,MAX_ARM_SUU=3,MAX_SHOT_TYPE=3};
	enum pShotType{MAIN_FIRE,MAIN_BEAM,MAIN_BEAM2,SUB_MISSILE=0,SUB_MISSLE2,SUB_BOOMERANG,ULT_BOMB=0,ULT_MISSILE,NONE=-1};
	void PlayerJet::tokusyuSyori(armtype atype,pShotType type);
	void oneSecSyori();
	void playerInit();
	void shotSyori();
	void playerShotGen(armtype atype,pShotType type);
	shotd *shotData[MAX_ARM_SUU][MAX_SHOT_TYPE];
	
	void shotDataSet(armtype atype,pShotType shotN);
	pShotType nowShot[MAX_ARM_SUU];
	PlayerJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void update();
	//弾の配列ポインタをくれれば一つ作れる。
};