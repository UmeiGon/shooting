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
	case MAWARU:
		move = t2k::vec3(x0 + circle.r*cos(liveTimer * 5), y0 + circle.r*sin(liveTimer * 5), 0)-circle.pos;
		x0 += gm->debug->dTime*5;
		break;
	case SPIN:
		move = t2k::vec3(-speed, -speed / 2, 0);
		angle += gm->debug->dTime;
		spined = true;
		break;
	case NAMI:
		move =t2k::vec3(-100.0f, 100*cos(liveTimer*3),0)*gm->debug->dTime;
		break;
	case YOKO:
		move = t2k::vec3(-150.0f,0,0)*gm->debug->dTime;
		if (circle.pos.y > gm->battleHeight) {
			move.y = 100.0f*gm->debug->dTime;
		}
		break;
	case HUYUU:
		 circle.pos=t2k::vec3((gm->winWidth/2)+(gm->winWidth/2)*sin(liveTimer), 550, 0);
		 return;
		break;
	default:
		break;
	}
	JetManager* jm = JetManager::getInstance();
	
	if (jm->ultActive == PlayerJet::ULT_HOLE&&stat==LIVE) {
		move = jm->ultpos - circle.pos;
		move = t2k::vec3Normalize(move);
		move * 400 * gm->debug->dTime;
	}
	circle.pos += move;
	if (capsule) {
		capTuizyu(move, spined);
	}

};
EnemyJet::EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx,int movet,int shott ,float sT, float dT) :Jet(X, Y, Angle, Size, Spd, Health, As, gfx, dT) {
	x0 = X;
	y0 = Y;
	shotType = shott;
	spawnTimer = sT;
	moveType = movet;
}
