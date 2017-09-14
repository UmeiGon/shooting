#include "TamaJet.h"
#include "math.h"
#include "gameManager.h"
#include "Dxlib.h"
#include "capsule.h"
#include "playerJet.h"
#include "enemyJet.h"
#include <typeinfo.h>
Maru::Maru(t2k::vec3 Pos, int R) {
	pos = Pos;
	r = R;
}
Maru::Maru() {}
Tama::Tama(float X, float Y, float Angle, float Size, float Spd, float atk, int gfx, float dtimer) {
	handle = gfx;
	deathTime = dtimer;
	liveTimer = 0;
	circle = Maru(t2k::vec3(X, Y, 0), Size / 2);
	startPos = t2k::vec3(X, Y, 0);
	size = Size;
	angle = Angle;
	Attack = atk;
	speed = Spd;
	capsule = nullptr;
	int gx;
	int gy;
	graphAngle = 0;
	GetGraphSize(handle, &gx, &gy);

	if (gy < gx) {
		graphSize = size / gy;
		addCapsuleHantei(gx*graphSize - size);
	}
	else {
		graphSize = size / gx;
	}
	gm = GameManager::getInstance();
}
void Tama::gAngleTyosei(float ang) {
	graphAngle = ang;
}
Tama::Tama() {
	gm = GameManager::getInstance();
}

void Tama::addCapsuleHantei(int nagasa) {
	int han = nagasa / 2;

	capsule = new Capsule(t2k::vec3(circle.pos.x + han, circle.pos.y, 0), t2k::vec3(circle.pos.x - han, circle.pos.y, 0), circle.r);
}
bool Tama::maruHantei(Maru a, Maru b) {
	int dis = t2k::vec3Distance(a.pos, b.pos);
	int rad = a.r + b.r;
	return(dis <= rad);
}
//画面内にいたらtrueを返す。
bool Tama::screenInside(float px, float py, float sabun) {
	if (-sabun <= px && px <= gm->battleWidth + sabun && -sabun <= py&&py <= gm->battleHeight + sabun) {
		return true;
	}
	else {
		return false;
	}
}
void Tama::LINE(t2k::vec3 a, t2k::vec3 b, int col) {
	DrawLine(a.x, a.y, b.x, b.y, col);
}
void Tama::draw() {
	liveTimer += gm->debug->dTime;
	if (!(handle == -1)) {
		DrawRotaGraphF(circle.pos.x, circle.pos.y, graphSize, angle + graphAngle, handle, TRUE);
	}
	if (gm->debug->showDebug) {
		if (capsule) {
			t2k::vec3 abTaniVec = t2k::vec3Normalize(t2k::vec3(capsule->b.x - capsule->a.x, capsule->b.y - capsule->a.y, 0));//線ABの単位ベクトルをノーマライズで求める
			t2k::vec3 test1(-abTaniVec.y, abTaniVec.x, 0);//90度回転
			test1 *= capsule->r;
			LINE(capsule->a + test1, capsule->b + test1, GetColor(0, 255, 0));
			LINE(capsule->a + -test1, capsule->b + -test1, GetColor(0, 255, 0));
			DrawCircle(capsule->a.x, capsule->a.y, capsule->r, GetColor(0, 255, 0), 0);
			DrawCircle(capsule->b.x, capsule->b.y, capsule->r, GetColor(0, 255, 0), 0);
		}
		else {
			DrawCircle(circle.pos.x, circle.pos.y, circle.r, GetColor(0, 255, 0), 0);
		}

	}
}


Tama::~Tama() {
	gm = nullptr;
	SAFE_DELETE(capsule);
}
Bullet::Bullet(float X, float Y, float Angle, float Size, float Spd, float atk, int gfx, float dtimer, float katuteikati) :Tama(X, Y, Angle, Size, Spd, atk, gfx, dtimer) {
	kantuTeika = katuteikati;
	atkedJet = nullptr;
	homingJet = nullptr;
	razerTimer = -1;
	animTimer = -1;
}
Bullet::~Bullet() {
	atkedJet = nullptr;
	homingJet = nullptr;
}
void Jet::drawHp(float startX, float startY, int Hp, int maxHp) {
	gm->drawBar(startX, startY, 45, 5, Hp, maxHp, GetColor(255, 0, 0), GetColor(0, 255, 0));
}
//画像をの角度を調節したい時はimgAngを変更。
//基本的な弾の発射
Bullet* Jet::shotGenHontai(float siz, float spd, float atk, int gfx, bool hou, int tx, int ty, float dt, float kantuteikati) {
	float fAng = 0;//弾の向き
	if (tx != -1) {
		float xsa = (tx - circle.pos.x);
		float ysa = (ty - circle.pos.y);
		fAng = atan2(ysa, xsa);
		if (houdai&&hou) {
			houdai->angle = fAng;
		}
	}
	float outx = circle.pos.x;
	float outy = circle.pos.y;
	if (hou&&houdai) {
		outx += houdai->x;
		outy += houdai->y;
	}
	for (int i = 0; i < JetManager::MAX_SHOT_SUU; i++) {
		if (!Shot[i]) {
			return Shot[i] = new Bullet(outx, outy, fAng, siz, spd, atk, gfx, dt, kantuteikati);
			break;
		}
	}
}
Bullet* Jet::shotGen(shotd s, bool houdaiShot, int targetX, int targetY) {
	return shotGenHontai(s.size, s.speed, s.atk, s.gfx, houdaiShot, targetX, targetY, s.deathTime, s.kantuTeika);
}
Bullet* Jet::shotGen(StandardShotTypes s, bool houdaiShot, int targetX, int targetY) {

	JetManager* jm = JetManager::getInstance();
	switch (s)
	{
	case EnemyJet::FIRE:
		return shotGenHontai(15, 10, 2, jm->shotGfx[JetManager::FIRE1], houdaiShot, targetX, targetY);
		break;
	case EnemyJet::BEAM:
		return shotGenHontai(13, 15, 1, jm->shotGfx[JetManager::BEAM1], houdaiShot, targetX, targetY);
		break;
	case EnemyJet::MISSILE1:
		return shotGenHontai(18, 12, 4, jm->shotGfx[JetManager::MISS1], houdaiShot, targetX, targetY);
		break;
	case EnemyJet::MISSILE2:
		return shotGenHontai(22, 2, 6, jm->shotGfx[JetManager::MISS2], houdaiShot, targetX, targetY);
		break;
	default:
		break;
	}
}



Jet::~Jet() {
	SAFE_DELETE(houdai);
	for (int i = 0; i < JetManager::MAX_SHOT_SUU; i++) {
		if (Shot[i]) {
		SAFE_DELETE(Shot[i]);
		}
	}
}
void Jet::addHoudai(int plusX, int plusY, float ang, int img, float hsize, float gAng) {
	houdai = new houData{ plusX,plusY,ang,img,hsize,gAng };
}

Jet::Jet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx, float dt) :Tama(X, Y, Angle, Size, Spd, 1, gfx, dt) {
	maxhealth = Health;
	health = Health;
	AttackSpeed = As;
	memset(Shot, 0, sizeof(Shot));
	houdai = nullptr;
	stat = TAIKI;
	atkTimer = 0;
}
void Jet::drawJet() {
	gm->debug->objSuu++;
	atkTimer += gm->debug->dTime;
	draw();
	if (houdai) {
		DrawRotaGraph(circle.pos.x + houdai->x, circle.pos.y + houdai->y, houdai->gSize, houdai->angle + houdai->gAngle, houdai->img, true);
	}
}


//返り値にmove値を返す。特殊弾:ちゃんとした返り値,普通の弾:-1,-1,-1の返り値,弾をデリート:-2,-2,-2の返り値。
t2k::vec3 Jet::shotMoveTokusyu(int sn) {
	JetManager* jm = JetManager::getInstance();
	if (Shot[sn]->handle == jm->shotGfx[JetManager::BOOMERANG]) {
		Shot[sn]->graphAngle += gm->debug->dTime*Shot[sn]->speed * 5;
		t2k::vec3 houkou(1, 1, 0);
		float  value = Shot[sn]->liveTimer *Shot[sn]->speed / 9.0f;
		if (value > 1) {
			//曲線処理が終わったら、最初だけプレイヤーの方向を向く。
			if (!(Shot[sn]->startPos == t2k::vec3(-500, -500, -500))) {
				Shot[sn]->angle = atan2(circle.pos.y - Shot[sn]->circle.pos.y, circle.pos.x - Shot[sn]->circle.pos.x);
				Shot[sn]->startPos = t2k::vec3(-500, -500, -500);
			}
			if (maruHantei(Shot[sn]->circle, circle)) {
				if (this == jm->player) {
					jm->player->tokusyuSyori(PlayerJet::SUB, PlayerJet::SUB_BOOMERANG);
				}
				return t2k::vec3(-2, -2, -2);
			}
			return t2k::vec3(t2k::vec3(cos(Shot[sn]->angle), sin(Shot[sn]->angle), 0) * 200 * Shot[sn]->speed*gm->debug->dTime);
		}
		//左上、左下、右下、右上（デフォ）
		if (Shot[sn]->angle < -MY_PI / 2) {
			houkou.x *= -1;
		}
		else if (Shot[sn]->angle < 0) {
			houkou;
		}
		else if (Shot[sn]->angle < MY_PI / 2) {
			houkou.y *= -1;
		}
		else {
			houkou *= -1;
		}


		return (t2k::vec3BezierSpline(Shot[sn]->startPos, Shot[sn]->startPos + t2k::vec3(300, -400, 0)*houkou, Shot[sn]->startPos + t2k::vec3(600, -700, 0)*houkou, Shot[sn]->startPos + t2k::vec3(800, 200, 0)*houkou, value) - Shot[sn]->circle.pos);
		//x800y300
	}
	//子機特殊処理
	if (Shot[sn]->handle == jm->shotGfx[JetManager::KOKI]) {
		for (int i = 0; i < PlayerJet::MAX_KOKI_SUU; i++) {
			if (Shot[sn] == jm->player->koki[i]) {
				t2k::vec3 mov = t2k::vec3(circle.pos.x + 40 * cos(MY_PI / 2 * i + liveTimer *4.5f), circle.pos.y + 40 * sin(MY_PI / 2 * i + liveTimer *4.5f), 0) - Shot[sn]->circle.pos;
				Shot[sn]->angle = atan2(mov.y, mov.x);
				return mov;
			}
		}
	}
	//ホーミング特殊処理
	if (Shot[sn]->homingJet) {
		t2k::vec3 e = Shot[sn]->homingJet->circle.pos;
		DrawRotaGraph(e.x, e.y, 1.0, 0, jm->gfx[JetManager::TARGET], true);
		t2k::vec3 at = Shot[sn]->homingJet->circle.pos - Shot[sn]->circle.pos;
		Shot[sn]->angle = atan2(at.y, at.x);
		at = t2k::vec3Normalize(at);
		return at*Shot[sn]->speed * 50 * gm->debug->dTime;
	}
	//レーザー特殊処理
	if (Shot[sn]->razerTimer >= 0) {
		Shot[sn]->razerTimer += gm->debug->dTime;
		DrawFormatString(0, 0, 0xffffff, "%f", Shot[sn]->razerTimer);
	}
	if (Shot[sn]->animTimer >= 0) {
		Shot[sn]->animTimer += gm->debug->dTime;
		if (Shot[sn]->animTimer > 0.13f) {
			for (int i = 0; i < jm->RAZER_ANIM_SUU; i++) {
				if (Shot[sn]->handle == jm->razerAnim[i]) {
					if (i == jm->RAZER_ANIM_SUU - 1) {
						Shot[sn]->animTimer = -1;
						break;
					}
					Shot[sn]->handle = jm->razerAnim[i + 1];
					Shot[sn]->animTimer = 0;
					break;
				}
			}
		}
	}
	return t2k::vec3(-1, -1, -1);
}



void Jet::drawMoveShot(int shotN) {
	//デスタイムが設定してあるor画面内で弾処理
	if (screenInside(Shot[shotN]->circle.pos.x, Shot[shotN]->circle.pos.y, Shot[shotN]->size) || Shot[shotN]->deathTime > 0) {

		t2k::vec3 move = shotMoveTokusyu(shotN);
		//デスタイムを超過または-2の返り値でデリート
		if (move == t2k::vec3(-2, -2, -2) || 0 < Shot[shotN]->deathTime&&Shot[shotN]->deathTime < Shot[shotN]->liveTimer) {
			SAFE_DELETE(Shot[shotN]);
		}
		else {
			if (move == t2k::vec3(-1, -1, -1)) {
				move = t2k::vec3(cos(Shot[shotN]->angle), sin(Shot[shotN]->angle), 0)* Shot[shotN]->speed * 100 * gm->debug->dTime;
			}
			JetManager* jm = JetManager::getInstance();
			switch (jm->ultActive)
			{
			case PlayerJet::ULT_BOMB:
				if (this != jm->player) {
					SAFE_DELETE(Shot[shotN]);
				}
				break;
			case PlayerJet::ULT_HOLE:
				if (this != jm->player) {
					if (t2k::vec3Distance(jm->ultpos, Shot[shotN]->circle.pos) < 3.0f) {
						SAFE_DELETE(Shot[shotN])
							break;
					}
					move = jm->ultpos - Shot[shotN]->circle.pos;
					move = t2k::vec3Normalize(move);
					move *= 400 * gm->debug->dTime;
				}
				break;
			default:
				break;
			}
			if (Shot[shotN]) {
				Shot[shotN]->circle.pos += move;
				Shot[shotN]->capTuizyu(move, true);
				Shot[shotN]->draw();
				gm->debug->objSuu++;
			}
		}
	}
	else {
		SAFE_DELETE(Shot[shotN])
	}
}


//カプセルをムーブに追従させる。
void Tama::capTuizyu(t2k::vec3 Move, bool spined) {
	if (!capsule)return;
	capsule->a += Move;
	capsule->b += Move;
	if (spined) {
		float wid = t2k::vec3Distance(capsule->a, capsule->b) / 2;
		t2k::vec3 spin(wid*cos(angle), wid*sin(angle), 0);
		capsule->a = circle.pos + spin;
		capsule->b = circle.pos - spin;
	}

}


shotd::shotd(float Siz, float Spd, float Atk, float Cd, float Heat, int Gfx, float dt, float kantuteikati) {
	kantuTeika = kantuteikati;
	size = Siz;
	speed = Spd;
	atk = Atk;
	shotCd = Cd;
	shotHeat = Heat;
	gfx = Gfx;
	deathTime = dt;
}

