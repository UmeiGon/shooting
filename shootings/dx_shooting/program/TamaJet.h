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
	float health;
	float maxhealth;
	Capsule *capsule;
	GameManager *gm;
	float graphAngle;
	t2k::vec3 startPos;
	float Attack = 1;
	float liveTimer;
	float deathTime;
	float speed;
	enum Tag { PLAYER, ENEMY };
	Maru circle;
	int handle;
	float angle;
	float size = 1;
	float graphSize;
	Tama();
	Tama(float X, float Y, float Angle, float Size, float Spd, float atk, int gfx,float dtimer=-1);
	static bool maruHantei(Maru a, Maru b);
	static void LINE(t2k::vec3 a, t2k::vec3 b, int col);
	bool screenInside(float px, float py, float sabun);
	void gAngleTyosei(float ang);
	void addCapsuleHantei(int nagasa);
	void capTuizyu(t2k::vec3 Move, bool spined);
	virtual ~Tama();

	void draw();
};

class Bullet:public Tama {
public:
	Bullet(float X, float Y, float Angle, float Size, float Spd, float atk, int gfx,float dtimer = -1,float kantuteikati=-1);
	float kantuTeika;
	float razerTimer;
	float animTimer;
	Tama* atkedJet;
	Jet* homingJet;
	~Bullet();
};

class shotd {
public:
	shotd(float Siz, float Spd, float Atk, float Cd, float Heat, int Gfx, float dt = -1,float kantuteikati=-1);
	float size;
	float speed;
	float atk;
	float shotCd;
	float shotHeat;
	float kantuTeika;
	int gfx;
	float deathTime;
};
class Jet :public Tama {
public:
	float atkTimer = 0;
	int handle2;
	float AttackSpeed = 1;
	enum StandardShotTypes { FIRE, BEAM, MISSILE1, MISSILE2 };
	houData *houdai;
	//生きていたらlive、スポーン前はtaiki、ライフ0で弾が画面にある場合dead、その後弾が無くなったらsyoukyo
	enum zyoutai { TAIKI, LIVE, DEAD, SYOUKYO };
	zyoutai stat = TAIKI;
	Bullet *Shot[JetManager::MAX_SHOT_SUU];
	Jet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx,float dt);
	t2k::vec3 shotMoveTokusyu(int sn);
	void drawJet();
	Bullet* shotGen(shotd s, bool houdaiShot = false, int targetX=-1, int targetY=-1);
	Bullet* shotGen(StandardShotTypes s, bool houd = false, int targetX = -1, int targetY = -1);
	void drawHp(float startX, float startY, int Hp, int maxHp);
	void drawMoveShot(int i);
	void addHoudai(int plusX, int plusY, float ang, int img, float hsize, float gAng);
	~Jet();
protected:
	Bullet* shotGenHontai(float siz,float speed,float atk,int gfx,bool hou, int tx,int ty,float dt=-1,float kantuteikati=-1);
};

