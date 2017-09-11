#include "playerJet.h"
#include "Dxlib.h"
#include "gameManager.h"
#include "jetManager.h"
#include "capsule.h"
#include "enemyJet.h"
#include <typeinfo.h>
PlayerJet::PlayerJet(float X, float Y, float Angle, float Size, float Spd, float Health, float As, int gfx) : Jet(X, Y, Angle, Size, Spd, Health, As, gfx, 0)
{
	energyPoint = 0;
	eisyo = 0;
	heat = 0;
	maxHeat = 100;
	hounetu = 0.5f;
	oneSecTimer = 0;
	overHeatTime = OVERHEAT_SEC;
	subAtkTimer = 0;
	memset(shotData, 0, sizeof(shotData));
	memset(koki, 0, sizeof(koki));
	memset(runes,0,sizeof(runes));
	ironsuu = 0;
	shotDataSet(MAIN, MAIN_FIRE);
	shotDataSet(MAIN, MAIN_BEAM);
	shotDataSet(MAIN, MAIN_BEAM2);
	shotDataSet(SUB, SUB_KOKI);
	shotDataSet(SUB, SUB_MISSLE);
	shotDataSet(SUB, SUB_BOOMERANG);
	shotDataSet(SUB, SUB_RAZER);
	shotDataSet(ULT,ULT_HOLE);
	shotDataSet(ULT,ULT_BOMB);
	nowShot[MAIN] = MAIN_FIRE;
	nowShot[SUB] = SUB_RAZER;
	nowShot[ULT] = ULT_BOMB;
}

void PlayerJet::tokusyuSyori(armtype atype, pShotType type) {
	switch (atype)
	{
	case PlayerJet::MAIN:
		switch (type)
		{
		case PlayerJet::MAIN_FIRE:
			break;
		case PlayerJet::MAIN_BEAM:
			break;
		case PlayerJet::MAIN_BEAM2:
			break;
		default:
			break;
		}
		break;
	case PlayerJet::SUB:
		switch (type)
		{

		case PlayerJet::SUB_KOKI:
			break;
		case PlayerJet::SUB_MISSLE:
			break;
		case PlayerJet::SUB_BOOMERANG:
			subAtkTimer += 5.0f;
			break;
		default:
			break;
		}
		break;
	case PlayerJet::ULT:
		switch (type)
		{

		case PlayerJet::ULT_BOMB:
			break;
		case PlayerJet::ULT_HOLE:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
void PlayerJet::playerShotGen(armtype atype, pShotType type) {

	Bullet* bu = nullptr;
	if (atype == MAIN) {
		bu = shotGen(*shotData[atype][type]);
	}
	else if (atype == SUB)
	{
		bu = shotGen(*shotData[atype][type], true, gm->cursor->mouseX, gm->cursor->mouseY);
	}
	else {
	}
	//特殊処理
	switch (atype)
	{
	case PlayerJet::MAIN:
		switch (type)
		{
		case PlayerJet::MAIN_FIRE:
			//3WAY
			if (heat > 50) {
				shotGen(*shotData[atype][type], false, circle.pos.x + 100, circle.pos.y - 10);
				shotGen(*shotData[atype][type], false, circle.pos.x + 100, circle.pos.y + 10);
			}
			break;
		case PlayerJet::MAIN_BEAM:
			if (heat > 50) {
				Tama* a = shotGen(*shotData[atype][type]);

				a->circle.pos.y += size / 3;
				bu->circle.pos.y -= size / 3;
			}
			break;
		case PlayerJet::MAIN_BEAM2:
			if (heat > 50) {
				Tama* a = shotGen(*shotData[atype][type]);
				a->circle.pos.y -= size / 2 * cos(liveTimer*3.0f);
				bu->circle.pos.y += size / 2 * cos(liveTimer*3.0f);
			}
			break;
		default:
			break;
		}
		break;
	case PlayerJet::SUB:

		switch (type)
		{
		case PlayerJet::SUB_KOKI:
			for (int i = 0; i < MAX_KOKI_SUU; i++) {
				if (!koki[i]) {
					koki[i] = bu;
					koki[i]->maxhealth = -100;
					koki[i]->graphAngle = MY_PI;
					return;
				}
			}
			break;
		case PlayerJet::SUB_MISSLE: {
			JetManager* jm = JetManager::getInstance();
			t2k::vec3 mouse(gm->cursor->mouseX, gm->cursor->mouseY, 0);
			int homingNum = -1;
			int homingDis = 10000;
			for (int e = 0; e < JetManager::MAX_ENEMY_SUU; e++) {
				if (jm->enemy[e]) {
					int dis = t2k::vec3Distance(mouse, jm->enemy[e]->circle.pos);
					if (dis < homingDis) {
						homingDis = dis;
						homingNum = e;
					}
				}
			}
			if (homingNum >= 0) {
				bu->homingJet = jm->enemy[homingNum];
			}
			bu->graphAngle += MY_PI;
			break;
		}
		case PlayerJet::SUB_BOOMERANG:
			break;
		case PlayerJet::SUB_RAZER:
			bu->circle.pos.x += size + t2k::vec3Distance(bu->capsule->a, bu->capsule->b) / 2;
			bu->animTimer = 0;
			bu->razerTimer = 0;
			bu->angle = 0;
			eisyo = 3.0f;
			break;
		default:
			break;
		}
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
	memset(koki, 0, sizeof(koki));
	circle.pos = t2k::vec3(gm->battleWidth / 2, gm->battleHeight / 2, 0);
	health = maxhealth;
	heat = 0;
	oneSecTimer = 0;
	liveTimer = 0;
	subAtkTimer = 0;
	eisyo = 0;
	energyPoint = 0;
	atkTimer = 0;
	overHeatTime = OVERHEAT_SEC;

}


void PlayerJet::shotSyori() {
	if (heat < 100 && GetMouseInput()&MOUSE_INPUT_LEFT&&atkTimer >= shotData[MAIN][nowShot[MAIN]]->shotCd) {
		playerShotGen(MAIN, nowShot[MAIN]);
		atkTimer = 0;
		heat += shotData[MAIN][nowShot[MAIN]]->shotHeat;
	}
	if (nowShot[SUB] == SUB_KOKI) {
		if (subAtkTimer > shotData[SUB][nowShot[SUB]]->shotCd) {
			for (int i = 0; i < MAX_KOKI_SUU; i++) {
				if (!koki[i]) {
					playerShotGen(SUB, nowShot[SUB]);
					subAtkTimer = 0;
					break;
				}
			}

		}
		if (heat < 100 && gm->input->isMouseDownTrigger(MOUSE_INPUT_RIGHT)) {
			for (int i = 0; i < MAX_KOKI_SUU; i++) {
				if (koki[i]) {
					heat += shotData[SUB][nowShot[SUB]]->shotHeat;
					t2k::vec3 mouse(gm->cursor->mouseX, gm->cursor->mouseY, 0);
					t2k::vec3 at(mouse - koki[i]->circle.pos);
					koki[i]->angle = atan2(at.y, at.x);
					koki[i] = nullptr;
					break;
				}
			}
		}
	}
	else if (heat < 100 && gm->input->isMouseDownTrigger(MOUSE_INPUT_RIGHT) && subAtkTimer >= shotData[SUB][nowShot[SUB]]->shotCd) {
		playerShotGen(SUB, nowShot[SUB]);
		subAtkTimer = 0;
		heat += shotData[SUB][nowShot[SUB]]->shotHeat;
	}
}
//playerのshotdataに中身を入れる。・・・メモリ節約のため
void PlayerJet::shotDataSet(armtype atype, pShotType shotN) {
	if (shotData[atype][shotN])return;
	JetManager* jm = JetManager::getInstance();
	switch (atype)
	{
	case PlayerJet::MAIN:
		switch (shotN)
		{
		case PlayerJet::MAIN_FIRE:
			shotData[MAIN][shotN] = new shotd(15, 10, 3.0f, 0.2f, 0.5f, jm->shotGfx[JetManager::FIRE1]);
			break;
		case PlayerJet::MAIN_BEAM:
			shotData[MAIN][shotN] = new shotd(5, 12, 1.2f, 0.1f, 0.25f, jm->shotGfx[JetManager::BEAM1]);
			break;
		case PlayerJet::MAIN_BEAM2:
			shotData[MAIN][shotN] = new shotd(3, 20, 0.5f, 0.05f, 0.15f, jm->shotGfx[JetManager::BEAM2]);
			break;
		default:
			break;
		}
		break;
	case PlayerJet::SUB:
		switch (shotN)
		{

		case PlayerJet::SUB_KOKI:
			shotData[SUB][shotN] = new shotd(25, 24, 6, 6.0f, 4.0f, jm->shotGfx[JetManager::KOKI], 0);
			break;
		case PlayerJet::SUB_MISSLE:
			shotData[SUB][shotN] = new shotd(12, 8, 15, 15.0f, 10.0f, jm->shotGfx[JetManager::MISS2]);
			break;
		case PlayerJet::SUB_BOOMERANG:
			shotData[SUB][shotN] = new shotd(50, 3, 6, 10.0f, 3.0f, jm->shotGfx[JetManager::BOOMERANG], 6.0f, 0.8f);
			break;
		case PlayerJet::SUB_RAZER:
			shotData[SUB][shotN] = new shotd(80, 0, 1.0f, 3.0f, 40.0f, jm->razerAnim[0], 3.0f, 0);
			break;
		default:
			break;
		}
		break;
	case PlayerJet::ULT:
		switch (shotN)
		{

		case PlayerJet::ULT_BOMB:
			shotData[ULT][shotN] = new shotd(40, 0, 40.0f, 120,0, jm->shotGfx[JetManager::BOMB_ICON]);
			break;
		case PlayerJet::ULT_HOLE:
			shotData[ULT][shotN] = new shotd(40,0,0.0f,120,0,jm->shotGfx[JetManager::HOLE_ICON]);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
//playerの毎秒処理
void PlayerJet::update() {
	JetManager* jm = JetManager::getInstance();

	//デバッグコマンド
	if (gm->input->isKeyDownTrigger(KEY_INPUT_F4)) {
		atkTimer = 100;
		subAtkTimer = 100;
		energyPoint += 2;
	}
	subAtkTimer += gm->debug->dTime;
	oneSecTimer += gm->debug->dTime;
	if (eisyo > 0) {
		eisyo -= gm->debug->dTime;

	}
	else {
		//動く処理
		speed = 180 * gm->debug->dTime;
		t2k::vec3 move(0, 0, 0);
		if (CheckHitKey(KEY_INPUT_A) && 0 < circle.pos.x) {
			move.x = -1;
		}
		else if (CheckHitKey(KEY_INPUT_D) && gm->battleWidth > circle.pos.x) {
			move.x = 1;
		}
		if (CheckHitKey(KEY_INPUT_W) && 0 < circle.pos.y) {
			move.y = -1;
		}
		else if (CheckHitKey(KEY_INPUT_S) && gm->battleHeight > circle.pos.y) {
			move.y = 1;
		}

		moved = false;
		if (!(move == t2k::vec3(0, 0, 0)))moved = true;
		move=t2k::vec3Normalize(move);
		bool boosted = false;
		if (moved&&heat < 100 && CheckHitKey(KEY_INPUT_LSHIFT)) {
			speed *= 2;
			boosted = true;
			heat += 0.05f;
		}
		circle.pos += move*speed*1.5f;

		
		if (boosted) {

			SetDrawBright(0, 255, 255);
			for (int i = 3; i >= 1; i--) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 205 - i * 30);
				DrawRotaGraph(circle.pos.x - move.x * 2 * i*1.8f, circle.pos.y - move.y * 2 * i*1.8f, graphSize, angle, jm->gfx[JetManager::ZIKI_JET], true);
			}
			SetDrawBright(255, 255, 255);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		if (capsule) {
			capTuizyu(move*speed, false);
		}
	}
	//1秒毎の処理
	if (oneSecTimer > 1.0f) {
		oneSecSyori();
		oneSecTimer = 0;
	}
	//体力を表示
	DrawFormatString(0, gm->battleHeight, 0xffffff, "%f", health);
	//鉄の数を表示
	DrawFormatString(0,gm->battleHeight+20,0xffffff,"%d",ironsuu);
	//energy数を表示
	DrawFormatString(0,gm->battleHeight+40,0xffffff,"%d",energyPoint);
	//heatを表示
	gm->drawBar(20, 450, 200, 30, heat, maxHeat, GetColor(255, 0, 0), 0xffffff);
	DrawFormatString(20, 500, 0xffffff, "%d/%dHEAT", (int)heat, (int)maxHeat);
	//cdを表示
	if (nowShot[MAIN] >= 0)DrawFormatString(0, 40, 0xffffff, "メイン武器使えるまで:%f", (shotData[MAIN][nowShot[MAIN]]->shotCd - atkTimer > 0) ? shotData[MAIN][nowShot[MAIN]]->shotCd - atkTimer : 0);
	if (nowShot[SUB] >= 0)DrawFormatString(0, 80, 0xffffff, "サブ武器使えるまで:%f", (shotData[SUB][nowShot[SUB]]->shotCd - subAtkTimer > 0) ? shotData[SUB][nowShot[SUB]]->shotCd - subAtkTimer : 0);

	shotSyori();
	if (heat >= 100) {//オーバーヒート処理
		heat = 100;
		overHeatTime -= gm->debug->dTime;
		DrawFormatString(20, 400, GetColor(255, 0, 0), "OVERHEAT!");
		if (overHeatTime <= 0) {
			heat = 0;
			overHeatTime = OVERHEAT_SEC;
		}
	
	}
	if (energyPoint >= 2 && gm->input->isKeyDownTrigger(KEY_INPUT_R)) {
		energyPoint = 0;
		jm->Ultimate(t2k::vec3(gm->cursor->mouseX,gm->cursor->mouseY,0),nowShot[ULT]);
	}
	gm->drawBar(0, gm->battleHeight - 50, 100, 20, health, maxhealth, GetColor(0, 255, 0), GetColor(255, 0, 0));


	drawJet();

	if (moved) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawRotaGraph(circle.pos.x - 20, circle.pos.y + 5, 1.0f, MY_PI / 2 + MY_PI / 8, jm->gfx[JetManager::BOOST], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}