#include "enemyJet.h"
#include "gameManager.h"
#include "PlayerJet.h"
void EnemyJet::eneMove() {
	t2k::vec3 move;
	speed = gm->debug->dTime * 10;
	bool spined = false;
	if (gm->debug->showDebug) {
		gm->drawBar(circle.pos.x - circle.r, circle.pos.y - circle.r, circle.r * 2, circle.r / 2, health, maxhealth, GetColor(0, 255, 0), GetColor(255, 0, 0));

	}
	switch (moveType)
	{
	case 0:
		//pos = t2k::vec3(x0 + r*cos(Timer * 5), y0 + r*sin(Timer * 5), 0);
		break;
	case 1:
		move = t2k::vec3(-speed, -speed / 2, 0);
		angle += gm->debug->dTime;
		spined = true;
		break;
	default:
		break;
	}
	JetManager* jm = JetManager::getInstance();
	
	if (jm->ultActive == PlayerJet::ULT_HOLE) {
		move = jm->ultpos - circle.pos;
		move = t2k::vec3Normalize(move);
		move * 400 * gm->debug->dTime;
	}
	circle.pos += move;
	if (capsule) {
		capTuizyu(move, spined);
		//‰ñ“]ƒeƒXƒg
	}

};
EnemyJet::EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx, float sT, float dT) :Jet(X, Y, Angle, Size, Spd, Health, As, gfx, dT) {
	x0 = X;
	y0 = Y;
	spawnTimer = sT;
	moveType = NANAME;
}
