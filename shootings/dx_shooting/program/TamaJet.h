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
	int Attack = 1;
	float atkTimer = 0;
	float speed;
	Maru circle;
	int handle = -1;
	int c;
	float angle;
	float size = 1;
	float graphSize;
	Tama();
	Tama(float X, float Y, float Angle, float Size, int Col, float Spd, int atk, int gfx);
	static bool maruHantei(Maru a, Maru b);
	static void LINE(t2k::vec3 a, t2k::vec3 b, int col);
	bool screenInside(float px, float py, float sabun);
	void gAngleTyosei(float ang);
	void addCapsuleHantei(int nagasa);

	virtual ~Tama();

	void draw();
};




class Jet :public Tama {
public:
	int health = 1;
	int maxhealth = 1;
	int handle2;
	int graphSize = 2;
	float AttackSpeed = 1;
	float liveTimer;
	houData *houdai;
	//生きていたらlive、スポーン前はtaiki、ライフ0で弾が画面にある場合dead、その後弾が無くなったらsyoukyo
	enum zyoutai { TAIKI, LIVE, DEAD, SYOUKYO };
	zyoutai stat = TAIKI;
	Tama *Shot[JetManager::MAX_SHOT_SUU];
	Jet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void drawJet();
	void shotGen(JetManager::shottype, int targetX = -1, int targetY = -1);
	void drawHp(float startX, float startY, int Hp, int maxHp);
	void drawMoveShot(int i);
	void addHoudai(int plusX, int plusY, float ang, int img, float hsize, float gAng);
	~Jet();
};

class EnemyJet :public Jet {
public:
	float x0;
	float y0;
	float spawnTimer = 0;
	float deathTimer = 0;
	enum movetypes { MAWARU, NANAME, NAMI };
	movetypes moveType = MAWARU;
	EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx, float sT = 0, float dT = 20);
	void eneMove();
	void capTuizyu(t2k::vec3 Move);
};
typedef struct {
		float shotCd;
		int shotHeat;
	}ShotData;
class PlayerJet :public Jet {
public:
	float heat;
	float maxHeat;
	float hounetu;
	float oneSecTimer;
	float overHeat;
	JetManager::shottype nowshot;
	void oneSecSyori();
	void playerInit();
	ShotData shotdata[JetManager::MAX_SHOT_SUU];
	enum teisu { OVERHEAT_SEC = 3 };
	PlayerJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void update();
	//弾の配列ポインタをくれれば一つ作れる。
};