#pragma once
#include "vec3.h"
class PlayerJet;
class EnemyJet;
class Jet;
class JetManager {
public:
	static JetManager* getInstance();
	enum teisu {MAX_ENEMY_SUU = 10,MAX_SHOT_SUU = 100,MAX_TARGET_SUU=100};
	JetManager();
	PlayerJet* player;
	EnemyJet* enemy[MAX_ENEMY_SUU];
	Jet* targetJet[MAX_TARGET_SUU];
	void inToTarget(Jet* target);
	void clearTarget();
	void test2(Jet* shooter);
};