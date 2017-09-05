#include "TamaJet.h"
#include "math.h"
#include "debug.h"
#include "gameManager.h"
#include "Dxlib.h"
#include "capsule.h"
#include "cursor.h"
#include "keycon.h"
#include <typeinfo.h>
Maru::Maru(t2k::vec3 Pos, int R) {
	pos = Pos;
	r = R;
}
Maru::Maru() {}
Tama::Tama(float X, float Y, float Angle, float Size, float Spd, int atk, int gfx, float dtimer) {
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
void Jet::drawHp(float startX, float startY, int Hp, int maxHp) {
	gm->drawBar(startX, startY, 45, 5, Hp, maxHp, GetColor(255, 0, 0), GetColor(0, 255, 0));
}
//画像をの角度を調節したい時はimgAngを変更。
//基本的な弾の発射
void Jet::shotGenHontai(float siz, float spd, int atk, int gfx, bool hou, int tx, int ty, float dt) {
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
			Shot[i] = new Tama(outx, outy, fAng, siz, spd, atk, gfx, dt);
			break;
		}
	}
}
void Jet::shotGen(shotd s, bool houdaiShot, int targetX, int targetY) {
	shotGenHontai(s.size, s.speed, s.atk, s.gfx, houdaiShot, targetX, targetY, s.deathTime);
}
void Jet::shotGen(StandardShotTypes s, bool houdaiShot, int targetX, int targetY) {

	JetManager* jm = JetManager::getInstance();
	switch (s)
	{
	case EnemyJet::FIRE:
		shotGenHontai(15, 10, 2, jm->shotGfx[JetManager::FIRE1], houdaiShot, targetX, targetY);
		break;
	case EnemyJet::BEAM:
		shotGenHontai(13, 15, 1, jm->shotGfx[JetManager::BEAM1], houdaiShot, targetX, targetY);
		break;
	case EnemyJet::MISSILE1:
		shotGenHontai(18, 12, 4, jm->shotGfx[JetManager::MISS1], houdaiShot, targetX, targetY);
		break;
	case EnemyJet::MISSILE2:
		shotGenHontai(22, 2, 6, jm->shotGfx[JetManager::MISS2], houdaiShot, targetX, targetY);
		break;
	default:
		break;
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

	//drawHp(circle.pos.x - circle.r, circle.pos.y - circle.r, health, maxhealth);
}

void PlayerJet::tokusyuSyori(pShotType type) {
	switch (type)
	{
	case PlayerJet::MAIN_FIRE:
		break;
	case PlayerJet::MAIN_BEAM:
		break;
	case PlayerJet::MAIN_BEAM2:
		break;
	case PlayerJet::SUB_MISSILE:
		break;
	case PlayerJet::SUB_MISSLE2:
		break;
	case PlayerJet::SUB_BOOMERANG:
		subAtkTimer += 3.0f;
		break;
	case PlayerJet::ULT_BOMB:
		break;
	case PlayerJet::ULT_MISSILE:
		break;
	case PlayerJet::NONE:
		break;
	default:
		break;
	}
}

//-1,-1,-1を返したら普通の弾,-2,-2,-2を返したらdelete弾によっては特殊処理。
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
					jm->player->tokusyuSyori(PlayerJet::SUB_BOOMERANG);
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

	return t2k::vec3(-1, -1, -1);
}
void Jet::drawMoveShot(int shotN) {
	if (screenInside(Shot[shotN]->circle.pos.x, Shot[shotN]->circle.pos.y, Shot[shotN]->size) || Shot[shotN]->deathTime > 0) {
		//画面内の時の処理

		t2k::vec3 move = shotMoveTokusyu(shotN);
		if (move == t2k::vec3(-1, -1, -1)) {
			move = t2k::vec3(cos(Shot[shotN]->angle), sin(Shot[shotN]->angle), 0)* Shot[shotN]->speed * 100 * gm->debug->dTime;
		}
		if (move == t2k::vec3(-2, -2, -2)|| 0 < Shot[shotN]->deathTime&&Shot[shotN]->deathTime < Shot[shotN]->liveTimer) {
			SAFE_DELETE(Shot[shotN]);
		}
		else {
			Shot[shotN]->circle.pos += move;
			Shot[shotN]->capTuizyu(move, true);
			Shot[shotN]->draw();
			gm->debug->objSuu++;
		}
	}
	else {
		SAFE_DELETE(Shot[shotN])
	}
}
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
void EnemyJet::eneMove() {
	t2k::vec3 move;
	speed = gm->debug->dTime * 10;
	bool spined = false;
	switch (moveType)
	{
	case 0:
		//pos = t2k::vec3(x0 + r*cos(Timer * 5), y0 + r*sin(Timer * 5), 0);
		break;
	case 1:
		move = t2k::vec3(-speed, -speed / 2, 0);
		circle.pos += move;
		angle += gm->debug->dTime;
		spined = true;
		break;
	default:
		break;
	}
	if (capsule) {
		capTuizyu(move, spined);
		//回転テスト
	}

};
EnemyJet::EnemyJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx, float sT, float dT) :Jet(X, Y, Angle, Size, Spd, Health, As, gfx, dT) {
	x0 = X;
	y0 = Y;
	spawnTimer = sT;
	moveType = NANAME;
}

PlayerJet::PlayerJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx) : Jet(X, Y, Angle, Size, Spd, Health, As, gfx, 0)
{
	heat = 0;
	maxHeat = 100;
	hounetu = 0.5f;
	oneSecTimer = 0;
	overHeatTime = OVERHEAT_SEC;
	subAtkTimer = 0;
	memset(shotData, 0, sizeof(shotData));
	shotDataSet(MAIN_FIRE);
	shotDataSet(SUB_MISSILE);
	shotDataSet(MAIN_BEAM);
	shotDataSet(SUB_BOOMERANG);
	nowShot[MAIN] = MAIN_FIRE;
	nowShot[SUB] = SUB_MISSILE;

}


void PlayerJet::playerShotGen(pShotType type) {
	heat += shotData[type]->shotHeat;
	if (type < MAIN_SUU) {
		shotGen(*shotData[type]);
	}
	else if (type < MAIN_SUU + SUB_SUU)
	{
		shotGen(*shotData[type], true, gm->cursor->mouseX, gm->cursor->mouseY);
	}
	else {
	}
	//特殊処理
	switch (type)
	{
	case PlayerJet::MAIN_FIRE:
		//3WAY
		if (heat > 60) {
			shotGen(*shotData[type], false, circle.pos.x + 100, circle.pos.y - 10);
			shotGen(*shotData[type], false, circle.pos.x + 100, circle.pos.y + 10);
		}
		break;
	case PlayerJet::MAIN_BEAM:
		break;
	case PlayerJet::MAIN_BEAM2:
		break;
	case PlayerJet::SUB_MISSILE:
		break;
	case PlayerJet::SUB_MISSLE2:
		break;
	case PlayerJet::SUB_BOOMERANG:
		break;
	case PlayerJet::ULT_BOMB:
		break;
	case PlayerJet::ULT_MISSILE:
		break;
	case PlayerJet::NONE:
		break;
	default:
		break;
	}
}
void PlayerJet::oneSecSyori() {
	if (heat > 0 && overHeatTime == OVERHEAT_SEC) {
		heat -= hounetu;
	}

}
void PlayerJet::playerInit() {
	for (int s = 0; s < JetManager::MAX_SHOT_SUU; s++) {
		SAFE_DELETE(Shot[s]);
	}
	circle.pos = t2k::vec3(gm->winWidth / 2, gm->winWidth / 2, 0);
	health = maxhealth;
	heat = 0;
	oneSecTimer = 0;
	liveTimer = 0;
	subAtkTimer = 0;
	atkTimer = 0;
	overHeatTime = OVERHEAT_SEC;

}


void PlayerJet::shotSyori() {
	if (GetMouseInput()&MOUSE_INPUT_LEFT&&atkTimer >= shotData[nowShot[MAIN]]->shotCd) {
		playerShotGen(nowShot[MAIN]);
		atkTimer = 0;
	}
	if (heat < 100 && gm->input->isMouseDownTrigger(MOUSE_INPUT_RIGHT) && subAtkTimer >= shotData[nowShot[SUB]]->shotCd) {
		playerShotGen(nowShot[SUB]);
		subAtkTimer = 0;
	}
	int a = 0;
}
shotd::shotd(float Siz, float Spd, int Atk, float Cd, float Heat, int Gfx, float dt) {
	size = Siz;
	speed = Spd;
	atk = Atk;
	shotCd = Cd;
	shotHeat = Heat;
	gfx = Gfx;
	deathTime = dt;
}
//playerのshotdataに中身を入れる。・・・メモリ節約のため
void PlayerJet::shotDataSet(pShotType shotN) {
	if (shotData[shotN])return;
	JetManager* jm = JetManager::getInstance();
	switch (shotN)
	{
	case PlayerJet::MAIN_FIRE:
		shotData[shotN] = new shotd(15, 10, 5, 0.2f, 0.5f, jm->shotGfx[JetManager::FIRE1]);
		break;
	case PlayerJet::MAIN_BEAM:
		shotData[shotN] = new shotd(5, 12, 1.2f, 0.1f, 0.25f, jm->shotGfx[JetManager::BEAM1]);
		break;
	case PlayerJet::MAIN_BEAM2:
		shotData[shotN] = new shotd(3, 20, 0.2f, 0.05f, 0.15f, jm->shotGfx[JetManager::BEAM2]);
		break;
	case PlayerJet::SUB_MISSILE:
		shotData[shotN] = new shotd(10, 14, 6, 6.0f, 4.0f, jm->shotGfx[JetManager::MISS1]);
		break;
	case PlayerJet::SUB_MISSLE2:
		shotData[shotN] = new shotd(12, 8, 15, 15.0f, 10.0f, jm->shotGfx[JetManager::MISS2]);
		break;
	case PlayerJet::SUB_BOOMERANG:
		shotData[shotN] = new shotd(50, 3, 10, 10.0f, 3.0f, jm->shotGfx[JetManager::BOOMERANG], 6.0f);
		break;
	case PlayerJet::ULT_BOMB:
		break;
	case PlayerJet::ULT_MISSILE:
		break;
	case PlayerJet::NONE:
		break;
	default:
		break;
	}
}
void PlayerJet::update() {
	JetManager* jm = JetManager::getInstance();
	//1秒毎の処理
	subAtkTimer += gm->debug->dTime;
	oneSecTimer += gm->debug->dTime;
	if (oneSecTimer > 1.0f) {
		oneSecSyori();
		oneSecTimer = 0;
	}

	//heatを表示
	gm->drawBar(20, 450, 200, 30, heat, maxHeat, GetColor(255, 0, 0), 0xffffff);
	DrawFormatString(20, 500, 0xffffff, "%d/%dHEAT", (int)heat, (int)maxHeat);
	//クリックで弾を作る処理
	if (nowShot[MAIN] >= 0)DrawFormatString(0, 40, 0xffffff, "メイン武器使えるまで:%f", (shotData[nowShot[MAIN]]->shotCd - atkTimer > 0) ? shotData[nowShot[MAIN]]->shotCd - atkTimer : 0);
	if (nowShot[SUB] >= 0)DrawFormatString(0, 80, 0xffffff, "サブ武器使えるまで:%f", (shotData[nowShot[SUB]]->shotCd - subAtkTimer > 0) ? shotData[nowShot[SUB]]->shotCd - subAtkTimer : 0);
	if (heat < 100) {
		shotSyori();
	}
	else {//オーバーヒート処理
		overHeatTime -= gm->debug->dTime;
		DrawFormatString(20, 400, GetColor(255, 0, 0), "OVERHEAT!");
		if (overHeatTime <= 0) {
			heat = 0;
			overHeatTime = OVERHEAT_SEC;
		}
	}
	//動く処理
	speed = 180 * gm->debug->dTime;
	bool boosted = false;
	if (heat < 100 && CheckHitKey(KEY_INPUT_LSHIFT)) {
		speed *= 2;
		boosted = true;
		heat += 0.05f;
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

	moved = false;
	if (!(move == t2k::vec3(0, 0, 0)))moved = true;
	t2k::vec3Normalize(move);
	circle.pos += move*speed;
	gm->drawBar(0, gm->winHeight - 50, 100, 20, health, maxhealth, GetColor(0, 255, 0), GetColor(255, 0, 0));
	if (boosted) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		for (int i = 1; i <= 3; i++) {
			DrawRotaGraph(circle.pos.x - move.x*speed * i, circle.pos.y - move.y*speed * i, graphSize, angle, jm->gfx[JetManager::ZIKI_JET], true);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (capsule) {
		capTuizyu(move*speed, false);
	}

	drawJet();

	if (moved) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawRotaGraph(circle.pos.x - 20, circle.pos.y + 5, 1, MY_PI / 2 + MY_PI / 8, jm->gfx[JetManager::BOOST], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}