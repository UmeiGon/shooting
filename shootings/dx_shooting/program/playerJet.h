#pragma once
#include "TamaJet.h"
class PlayerJet :public Jet {
public:
	float heat;
	float maxHeat;
	float hounetu;
	float oneSecTimer;
	float overHeatTime;
	float subAtkTimer;
	int razerNum;
	int ultPoint;
	bool moved;
	float eisyo;
	enum armtype { MAIN, SUB, ULT };
	enum teisu { OVERHEAT_SEC = 3, MAX_ARM_SUU = 3, MAX_SHOT_TYPE = 4 ,MAX_KOKI_SUU=4};
	enum pShotType { MAIN_FIRE, MAIN_BEAM, MAIN_BEAM2, SUB_KOKI = 0, SUB_MISSLE, SUB_BOOMERANG,SUB_RAZER, ULT_BOMB = 0, ULT_HOLE, NONE = -1 };
	void PlayerJet::tokusyuSyori(armtype atype, pShotType type);
	void oneSecSyori();
	void playerInit();
	void shotSyori();
	void playerShotGen(armtype atype, pShotType type);
	shotd *shotData[MAX_ARM_SUU][MAX_SHOT_TYPE];
	Bullet* koki[MAX_KOKI_SUU];
	void shotDataSet(armtype atype, pShotType shotN);
	pShotType nowShot[MAX_ARM_SUU];
	PlayerJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx);
	void update();
};