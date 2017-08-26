#include "TamaJet.h"
#include "math.h"
#include "debug.h"
#include "gameManager.h"
#include "Dxlib.h"

Tama::Tama(float X, float Y, float Angle, float Size, int Col, float Spd, int atk = 1, int gfx = -1) {
	handle = gfx;
	pos.x = X;
	pos.y = Y;
	r = Size / 2;
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
}
void Tama::gAngleSet(float ang) {
	graphAngle = ang;
}
Tama::Tama() {
	gm = GameManager::getInstance();
}

bool Tama::maruHantei(Tama a, Tama b) {
	int base = a.pos.x - b.pos.x;
	int height = a.pos.y - b.pos.y;
	int rad = a.r + b.r;
	return(base*base + height*height <= rad*rad);
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
void Tama::draw() {
	Timer += gm->debug->dTime;

	if (!(handle == -1)) {
		DrawRotaGraphF(pos.x, pos.y, graphSize, angle+graphAngle, handle, TRUE);
	}
	else {
		DrawCircle(pos.x, pos.y, r, c);
	}

}
void Jet::drawHp(float startX, float startY, int Hp, int maxHp) {
	float H = (float)Hp / (float)maxHp;
	float tate = 5;
	float yoko = 45;
	DrawBox(startX, startY, startX + yoko, startY + tate, GetColor(255, 0, 0), true);
	DrawBox(startX, startY, startX + H * yoko, startY + tate, GetColor(0, 255, 0), true);
}
//‰æ‘œ‚ð‚ÌŠp“x‚ð’²ß‚µ‚½‚¢Žž‚ÍimgAng‚ð•ÏXB
void Jet::shotGen(int targetX, int targetY, int col, float spd, int atk, int size, int img,float imgAng) {
	float xsa = (targetX - pos.x);
	float ysa = (targetY - pos.y);
	float fAng = atan2(ysa, xsa);
	if (houdai) {
		houdai->angle=fAng;
	}
	for (int i = 0; i < JetManager::MAX_SHOT_SUU; i++) {
		if (Shot[i] == nullptr) {
			Shot[i] = new Tama(pos.x, pos.y, fAng, size, col, spd, atk, img);
			Shot[i]->gAngleSet(imgAng);
			Timer = 0;
			break;
		}
	}
}
Tama::~Tama() {
	gm = nullptr;
}
Jet::~Jet() {
	SAFE_DELETE(houdai);
	for (int i = 0; i < JetManager::MAX_SHOT_SUU&&Shot[i]; i++) {
		SAFE_DELETE(Shot[i]);
	}
}
void Jet::addHoudai(int plusX,int plusY,float ang,int img,float hsize,float gAng) {
	houdai = new houData{plusX,plusY,ang,img,hsize,gAng};
}
Jet::Jet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx):Tama(X, Y, Angle, Size,0xffffff,Spd,1,gfx) {
	maxhealth = Health;
	health = Health;
	AttackSpeed = As;
	memset(Shot, 0, sizeof(Shot));
	houdai = nullptr;
}
void Jet::drawJet() {
	gm->debug->objSuu++;
	draw();
	if (houdai) {
		DrawRotaGraph(pos.x + houdai->x, pos.y + houdai->y, houdai->gSize, houdai->angle+houdai->gAngle, houdai->img, true);
	}
	
	drawHp(pos.x - r, pos.y - r, health, maxhealth);
}
void Jet::drawMoveShot(int shotN) {
	if (screenInside(Shot[shotN]->pos.x, Shot[shotN]->pos.y, Shot[shotN]->size)) {
		//‰æ–Ê“à‚ÌŽž‚Ìˆ—
		Shot[shotN]->pos.x += cos(Shot[shotN]->angle) * Shot[shotN]->speed * 100 * gm->debug->dTime;
		Shot[shotN]->pos.y += sin(Shot[shotN]->angle) * Shot[shotN]->speed * 100 * gm->debug->dTime;
		Shot[shotN]->draw();
		gm->debug->objSuu++;
	}
	else {
		SAFE_DELETE(Shot[shotN]);
	}
}
void EnemyJet::eneMove() {
	switch (moveType)
	{
	case 0:
		//pos = t2k::vec3(x0 + r*cos(Timer * 5), y0 + r*sin(Timer * 5), 0);
		break;
	case 1:
		pos += t2k::vec3(-gm->debug->dTime * 10, gm->debug->dTime * 10, 0);
		break;
	default:
		break;
	}
};
EnemyJet::EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx = -1) :Jet(X, Y, Angle, Size, Spd, Health, As, gfx) {
	x0 = X;
	y0 = Y;
}

PlayerJet::PlayerJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx = -1) : Jet(X, Y, Angle, Size, Spd, Health, As, gfx) {}


void PlayerJet::pMove() {
	//“®‚­ˆ—
	speed = 180 * gm->debug->dTime;
	if (CheckHitKey(KEY_INPUT_LSHIFT)) {
		speed *= 2;
	}
	t2k::vec3 move(0, 0, 0);
	if (CheckHitKey(KEY_INPUT_A) && 0 < pos.x) {
		move.x = -1;
	}
	else if (CheckHitKey(KEY_INPUT_D) && gm->winWidth > pos.x) {
		move.x = 1;
	}
	if (CheckHitKey(KEY_INPUT_W) && 0 < pos.y) {
		move.y = -1;
	}
	else if (CheckHitKey(KEY_INPUT_S) && gm->winHeight > pos.y) {
		move.y = 1;
	}
	t2k::vec3Normalize(move);
	pos += move*speed;
}