#include "TamaJet.h"
#include "math.h"
#include "debug.h"
#include "gameManager.h"
#include "Dxlib.h"
#include "capsule.h"
Maru::Maru(t2k::vec3 Pos, int R) {
	pos = Pos;
	r = R;
}
Maru::Maru() {}
Tama::Tama(float X, float Y, float Angle, float Size, int Col, float Spd, int atk = 1, int gfx = -1) {
	handle = gfx;
	circle = Maru(t2k::vec3(X, Y, 0), Size / 2);
	size = Size;
	angle = Angle;
	c = Col;
	Attack = atk;
	speed = Spd;
	int gx;
	int gy;
	graphAngle = 0;
	GetGraphSize(handle, &gx, &gy);
	graphSize = size / gy;
	gm = GameManager::getInstance();
	capsule = nullptr;
}
void Tama::gAngleSet(float ang) {
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
bool Tama::screenInside(float px, float py, float sabun) {
	if (-sabun <= px && px <= gm->winWidth + sabun && -sabun <= py&&py <= gm->winHeight + sabun) {
		return true;
	}
	else {
		return false;
	}
	return false;
}
void Tama::LINE(t2k::vec3 a, t2k::vec3 b, int col) {
	DrawLine(a.x, a.y, b.x, b.y, col);
}
void Tama::draw() {
	atkTimer += gm->debug->dTime;

	if (!(handle == -1)) {
		DrawRotaGraphF(circle.pos.x, circle.pos.y, graphSize, angle + graphAngle, handle, TRUE);
	}
	else {
		DrawCircle(circle.pos.x, circle.pos.y, circle.r, c);
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
void Jet::drawHp(float startX, float startY, int Hp, int maxHp) {
	float H = (float)Hp / (float)maxHp;
	float tate = 5;
	float yoko = 45;
	DrawBox(startX, startY, startX + yoko, startY + tate, GetColor(255, 0, 0), true);
	DrawBox(startX, startY, startX + H * yoko, startY + tate, GetColor(0, 255, 0), true);
}
//画像をの角度を調節したい時はimgAngを変更。
void Jet::shotGen(int targetX, int targetY, int col, float spd, int atk, int size, int img, float imgAng) {
	float xsa = (targetX - circle.pos.x);
	float ysa = (targetY - circle.pos.y);
	float fAng = atan2(ysa, xsa);
	if (houdai) {
		houdai->angle = fAng;
	}
	for (int i = 0; i < JetManager::MAX_SHOT_SUU; i++) {
		if (Shot[i] == nullptr) {
			Shot[i] = new Tama(circle.pos.x, circle.pos.y, fAng, size, col, spd, atk, img);
			Shot[i]->gAngleSet(imgAng);
			atkTimer = 0;
			break;
		}
	}
}
Tama::~Tama() {
	gm = nullptr;
	SAFE_DELETE(capsule);
}
Jet::~Jet() {
	SAFE_DELETE(houdai);
	for (int i = 0; i < JetManager::MAX_SHOT_SUU&&Shot[i]; i++) {
		SAFE_DELETE(Shot[i]);
	}
}
void Jet::addHoudai(int plusX, int plusY, float ang, int img, float hsize, float gAng) {
	houdai = new houData{ plusX,plusY,ang,img,hsize,gAng };
}
Jet::Jet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx) :Tama(X, Y, Angle, Size, 0xffffff, Spd, 1, gfx) {
	maxhealth = Health;
	health = Health;
	AttackSpeed = As;
	memset(Shot, 0, sizeof(Shot));
	houdai = nullptr;
	stat = TAIKI;
	liveTimer = 0;
}
void Jet::drawJet() {
	gm->debug->objSuu++;
	draw();
	if (houdai) {
		DrawRotaGraph(circle.pos.x + houdai->x, circle.pos.y + houdai->y, houdai->gSize, houdai->angle + houdai->gAngle, houdai->img, true);
	}

	drawHp(circle.pos.x - circle.r, circle.pos.y - circle.r, health, maxhealth);
}
void Jet::drawMoveShot(int shotN) {
	if (screenInside(Shot[shotN]->circle.pos.x, Shot[shotN]->circle.pos.y, Shot[shotN]->size)) {
		//画面内の時の処理
		Shot[shotN]->circle.pos.x += cos(Shot[shotN]->angle) * Shot[shotN]->speed * 100 * gm->debug->dTime;
		Shot[shotN]->circle.pos.y += sin(Shot[shotN]->angle) * Shot[shotN]->speed * 100 * gm->debug->dTime;
		Shot[shotN]->draw();
		gm->debug->objSuu++;
	}
	else {
		SAFE_DELETE(Shot[shotN]);
	}
}
void EnemyJet::capTuizyu(t2k::vec3 Move) {
	capsule->a += Move;
	capsule->b += Move;
}
void EnemyJet::eneMove() {
	liveTimer += gm->debug->dTime;
	t2k::vec3 move;
	speed = gm->debug->dTime * 10;
	switch (moveType)
	{
	case 0:
		//pos = t2k::vec3(x0 + r*cos(Timer * 5), y0 + r*sin(Timer * 5), 0);
		break;
	case 1:
		move = t2k::vec3(-speed, -speed / 2, 0);
		circle.pos += move;
		break;
	default:
		break;
	}
	if (capsule) {
		capTuizyu(move);
		//回転テスト
		angle += gm->debug->dTime;
		float w = t2k::vec3Distance(capsule->a, capsule->b) / 2;
		t2k::vec3 spin(w*cos(angle), w*sin(angle), 0);
		capsule->a=circle.pos + spin;
		capsule->b=circle.pos - spin;
	}

};
EnemyJet::EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx, float sT, float dT) :Jet(X, Y, Angle, Size, Spd, Health, As, gfx) {
	x0 = X;
	y0 = Y;
	spawnTimer = sT;
	deathTimer = dT;
	moveType = NANAME;
}

PlayerJet::PlayerJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx) : Jet(X, Y, Angle, Size, Spd, Health, As, gfx) {}


void PlayerJet::pMove() {
	//動く処理
	speed = 180 * gm->debug->dTime;
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		speed *= 2;
	}
	t2k::vec3 move(0, 0, 0);
	if (CheckHitKey(KEY_INPUT_A) && 0 < circle.pos.x) {
		move.x = -1;
	}
	else if (CheckHitKey(KEY_INPUT_D) && gm->winWidth > circle.pos.x) {
		move.x = 1;
	}
	if (CheckHitKey(KEY_INPUT_W) && 0 < circle.pos.y) {
		move.y = -1;
	}
	else if (CheckHitKey(KEY_INPUT_S) && gm->winHeight > circle.pos.y) {
		move.y = 1;
	}
	t2k::vec3Normalize(move);
	circle.pos += move*speed;
}