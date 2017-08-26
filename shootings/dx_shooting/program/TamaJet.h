#pragma once
#include "vec3.h"
#include "jetManager.h"
class GameManager;
typedef struct {
	int x;
	int y;
	float angle;
	int img;
	float gSize;
	float gAngle = 0;
}houData;
class Tama {
public:
	GameManager *gm;
	float graphAngle;
	int Attack = 1;
	float Timer = 0;
	float speed;
	t2k::vec3 pos;
	float r;
	int handle = -1;
	int c;
	float angle;
	float size = 1;
	float graphSize;
	Tama(float X, float Y, float Angle, float Size, int Col, float Spd, int atk,int gfx);
	static bool maruHantei(Tama a,Tama b);
	bool screenInside(float px, float py, float sabun);
	void gAngleSet(float ang);
	virtual ~Tama();
	Tama();
	void draw();
};




class Jet :public Tama {
public:
	int health = 1;
	int maxhealth = 1;
	int handle2;
	int graphSize = 2;
	float AttackSpeed = 1;
	houData *houdai;
	Tama *Shot[JetManager::MAX_SHOT_SUU];
	Jet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void drawJet();
	void shotGen(int targetX, int targetY, int col, float spd,int atk=1, int size = 16,int img=-1,float imgAng=0);
	void drawHp(float startX, float startY, int Hp, int maxHp);
	void drawMoveShot(int i);
	void addHoudai(int plusX,int plusY,float ang,int img,float hsize,float gAng);
	~Jet();
};
class EnemyJet :public Jet {
public:
	float x0;
	float y0;
	float deathTimer = 0;
	int moveType = 0;
	EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void eneMove();
};
class PlayerJet :public Jet {
public:
	PlayerJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void pMove();
	//弾の配列ポインタをくれれば一つ作れる。
};