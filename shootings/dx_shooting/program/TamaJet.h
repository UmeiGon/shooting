#pragma once
#include "Dxlib.h"
#include "vec3.h"
#include "jetManager.h"
class GameManager;
class Tama {
public:
	GameManager *gm;
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
	Tama(float X, float Y, float Angle, float Size, int Col, float Spd, int gfx);
	static bool maruHantei(Tama a,Tama b);
	bool screenInside(float px, float py, float sabun);
	Tama();
	void draw();
};




class Jet :public Tama {
public:
	int health = 1;
	int maxhealth = 1;
	float AttackSpeed = 1;
	Tama *Shot[JetManager::MAX_SHOT_SUU];
	Jet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void drawJet();
	void shotGen(int targetX, int targetY, int col, float spd);
	void drawHp(float startX, float startY, int Hp, int maxHp);
	void drawMoveShot(int i);

};
class EnemyJet :public Jet {
public:
	float x0;
	float y0;
	float deathTimer = 0;
	int moveType = 0;
	EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void shotFunc(PlayerJet *pjet);
	void shotAtari(PlayerJet *pjet, int i);
	void eneMove();
};
class PlayerJet :public Jet {
public:
	PlayerJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void shotFunc();
	void shotAtari(int k, int i);
	void pMove();
	//弾の配列ポインタをくれれば一つ作れる。
};