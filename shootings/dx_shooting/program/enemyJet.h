#pragma once
#include "TamaJet.h"
class EnemyJet :public Jet {
public:
	float x0;
	float y0;
	float spawnTimer = 0;
	enum movetypes { MAWARU, SPIN, NAMI,YOKO,HUYUU };
	int moveType = MAWARU;
	int shotType;
	EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx,int movet,int shott, float sT = 0, float dT = 20);
	void eneMove();

};
