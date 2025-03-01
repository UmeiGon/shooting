#include "jetManager.h"
#include "gameManager.h"
#include "playerJet.h"
#include "enemyJet.h"
#include "TamaJet.h"
#include "debug.h"
#include "Animation.h"
#include "Dxlib.h"
#include "Capsule.h"
JetManager* JetManager::getInstance() {
	static JetManager* instance = nullptr;
	if (nullptr == instance) {
		instance = new JetManager();
	}
	return instance;
}
int JetManager::getGraphYsize(int img) {
	int a, b;
	GetGraphSize(img, &a, &b);
	return b;
}
void JetManager::init() {
	//jetmanagerのグラフィック初期化
	int a = LoadGraph("images/GunHound01.png");
	int b = LoadGraph("images/TexDummyHound.png");
	int c = LoadGraph("images/effect.png");
	int d = LoadGraph("images/razer.png");
	//jet本体のグラ
	gfx[BOOST] = DerivationGraph(15, 98, 33, 28, b);
	gfx[ZIKI_ZENTAI] = LoadGraph("images/player1.png");
	gfx[ZIKI_JET] = LoadGraph("images/playerRobo.png");
	gfx[ENEMY_ZENTAI] = LoadGraph("images/enemy1.PNG");
	gfx[ENEMY_HOUDAI] = DerivationGraph(313, 272, 77, 54, gfx[ENEMY_ZENTAI]);
	gfx[ENEMY_JET_TYPE1] = DerivationGraph(163, 72, 108, 42, gfx[ENEMY_ZENTAI]);
	gfx[TARGET] = LoadGraph("images/target.png");
	//アニメーショングラ
	LoadDivGraph("images/bakuhatsu01.png", 12, 3, 4, 64, 64, bomAnim);
	LoadDivGraph("images/zangeki.png", 9, 9, 1, 120, 120, zangekiAnim);
	LoadDivGraph("images/suikomu.png", 15, 15, 1, 312, 312, holeAnim);
	LoadDivGraph("images/bomb1.png", 8, 1, 8, 640, 240, ultbom1Anim);
	LoadDivGraph("images/bomb2.png", 9, 1, 9, 640, 240, ultbom2Anim);
	for (int i = 0; i < RAZER_ANIM_SUU; i++) {
		razerAnim[i] = DerivationGraph(0, 505 - (i + 1) * 72, 535, 62, d);
	}
	//アームのグラ
	armgfx[FIRE_WEAPON] = DerivationGraph(160, 106, 40, 13, a);
	armgfx[NORMAL_WEAPON] = DerivationGraph(160, 74, 32, 13, a);
	armgfx[BEAM_WEAPON] = DerivationGraph(160, 202, 48, 14, a);
	//弾のグラ
	shotGfx[FIRE1] = LoadGraph("images/fire.png");
	shotGfx[MISS1] = DerivationGraph(0, 24, 18, 5, gfx[ZIKI_ZENTAI]);
	shotGfx[BEAM1] = DerivationGraph(19, 25, 28, 3, gfx[ZIKI_ZENTAI]);
	shotGfx[MISS2] = DerivationGraph(50, 130, 47, 17, gfx[ENEMY_ZENTAI]);
	shotGfx[BEAM2] = DerivationGraph(33, 131, 13, 4, c);
	shotGfx[KOKI] = DerivationGraph(413, 59, 33, 28, gfx[ENEMY_ZENTAI]);
	shotGfx[BOOMERANG] = LoadGraph("images/Boomerang.png");
	shotGfx[BOMB_ICON] = DerivationGraph(194, 170, 59, 86, c);
	shotGfx[HOLE_ICON] = holeAnim[6];
	shotGfx[RAZER_ICON] = LoadGraph("images/razerIcon.png");


	DeleteGraph(a);
	DeleteGraph(b);
	DeleteGraph(c);
	DeleteGraph(d);
	//player生成
	GameManager* gm = GameManager::getInstance();
	player = new PlayerJet(gm->battleWidth / 2, gm->battleHeight / 2, 0, 50, 10, 3, 0.2, gfx[JetManager::ZIKI_JET]);
	player->addHoudai(-20, -15, 0, armgfx[JetManager::FIRE_WEAPON], 1, 0);
}
JetManager::JetManager() {
	ultActive = -1;
	memset(anims, 0, sizeof(anims));
	memset(enemy, 0, sizeof(enemy));
	memset(targetJet, 0, sizeof(targetJet));
	player = nullptr;
}
//ショットのアイコンを場所とサイズを指定して描画
void JetManager::shotIconDraw(int fx, int fy, int armtype, int type, int fsize) {
	if (type == PlayerJet::NONE || !player->shotData[armtype][type])return;
	GameManager* gm = GameManager::getInstance();
	DrawModiGraph(fx, fy, fx + fsize, fy, fx + fsize, fy + fsize, fx, fy + fsize, gm->UIImg[GameManager::SHOTFLAME], true);
	int sgfx = player->shotData[armtype][type]->gfx;
	if (armtype == PlayerJet::SUB&&type == PlayerJet::SUB_RAZER) {
		sgfx = shotGfx[RAZER_ICON];
	}
	int a, b;
	float drawsize;
	GetGraphSize(sgfx, &a, &b);
	int s = a > b ? a : b;
	drawsize = (float)fsize / ((float)s+12.0f);
	DrawRotaGraph(fx + fsize / 2, fy + fsize / 2, drawsize, 0, sgfx, true);
}
void JetManager::animationUpdate() {
	GameManager *gm = GameManager::getInstance();
	for (int i = 0; i < MAX_ANIM_SUU; i++) {
		if (anims[i]) {
			anims[i]->timer += gm->debug->dTime;
			if (anims[i]->img[(int)(anims[i]->timer / anims[i]->changeTime)] == 0) {
				SAFE_DELETE(anims[i]);
				continue;
			}
			DrawRotaGraph(anims[i]->pos.x, anims[i]->pos.y, anims[i]->size, 0, anims[i]->img[(int)(anims[i]->timer / anims[i]->changeTime)], true);
		}
	}
}
void JetManager::animStart(t2k::vec3 Pos, int Img[], float Size) {
	for (int i = 0; i < MAX_ANIM_SUU; i++) {
		if (!anims[i]) {
			anims[i] = new Animation(Pos, Img, Size);
			break;
		}
	}
}
//判定only
void JetManager::inToTarget(Tama* target) {
	for (int i = 0; i < MAX_TARGET_SUU; i++) {
		if (!targetJet[i]) {
			targetJet[i] = target;
			return;
		}
	}
}
void JetManager::clearTarget() {
	for (int i = 0; i < MAX_TARGET_SUU; i++) {
		targetJet[i] = nullptr;
	}
}
//弾の当たり判定の判断をして当たっているか調べる。
bool JetManager::hitHantei(Tama *a, Tama *b) {
	bool acol = false;
	bool bcol = false;
	if (a->capsule)acol = true;
	if (b->capsule)bcol = true;
	if (acol && bcol) {
		return Capsule::capsuleHantei(*a->capsule, *b->capsule);
	}
	else if (acol) {
		return Capsule::capmaruHantei(*a->capsule, b->circle.pos, b->circle.r);
	}
	else if (bcol) {
		return Capsule::capmaruHantei(*b->capsule, a->circle.pos, a->circle.r);
	}
	return Tama::maruHantei(a->circle, b->circle);
}
void JetManager::hitedEffect(Tama *hit) {
	SetDrawBright(255, 0, 0);
	DrawRotaGraph(hit->circle.pos.x, hit->circle.pos.y, hit->graphSize, hit->graphAngle + hit->angle, hit->handle, true);
	SetDrawBright(255, 255, 255);
}
void JetManager::UltimateUpdate() {
	static float ultLiveTimer = 0;
	static float ultAtkTimer = 0;
	static float animtimer = 0;
	static int  animnum = 0;
	GameManager* gm = GameManager::getInstance();
	switch (ultActive)
	{
	case PlayerJet::ULT_BOMB:
		if (ultLiveTimer == 0) {
			animStart(t2k::vec3(gm->battleWidth / 2, gm->battleHeight, 0), ultbom1Anim, 3.0f);
		}
		ultLiveTimer += gm->debug->dTime;
		ultAtkTimer += gm->debug->dTime;
		animtimer += gm->debug->dTime;
		if (ultAtkTimer >= 0.25f) {
			for (int i = 0; i < MAX_ENEMY_SUU; i++) {
				if (enemy[i] && enemy[i]->stat == EnemyJet::LIVE) {
					enemy[i]->health -= player->shotData[player->ULT][player->ULT_BOMB]->atk / 8;
				}
			}
			ultAtkTimer = 0;
		}
		if (animtimer >= 0.5f) {
			animStart(t2k::vec3(gm->battleWidth / 2, gm->battleHeight - animnum*300.0f, 0), ultbom2Anim, 3.0f);
			animnum++;
			animtimer = 0;
		}
		if (ultLiveTimer > 2.0f) {
			ultActive = -1;
		}

		break;
	case PlayerJet::ULT_HOLE:
		ultLiveTimer += gm->debug->dTime;
		ultAtkTimer += gm->debug->dTime;
		animtimer += gm->debug->dTime;
		DrawRotaGraph(ultpos.x, ultpos.y, 2.0f, 0, holeAnim[animnum], true);
		if (animtimer >= 0.1f) {
			animnum++;
			if (animnum >= HOLE_ANIM_SUU) {
				animnum = 0;
			}
			animtimer = 0;
		}
		if (ultLiveTimer > 4.0f) {
			ultActive = -1;
		}
		break;
	default:
		ultLiveTimer = 0;
		ultAtkTimer = 0;
		animtimer = 0;
		animnum = 0;
		break;
	}
}
void JetManager::Ultimate(t2k::vec3 pos, int num) {
	ultpos = pos;
	ultActive = num;
}
//シューターのショットをターゲット
void JetManager::damagesSyori(Jet* shooter) {
	if (!shooter)return;
	int count = 0;
	for (int s = 0; s < MAX_SHOT_SUU; s++) {
		if (shooter->Shot[s]) {
			count++;
			shooter->drawMoveShot(s);
			for (int t = 0; t < MAX_TARGET_SUU; t++) {
				if (shooter->Shot[s] && targetJet[t] && hitHantei(shooter->Shot[s], targetJet[t]) && (targetJet[t]->health > 0 || targetJet[t]->maxhealth == -100)) {
					if (shooter->Shot[s]->kantuTeika >= 0.0f) {
						if (shooter->Shot[s]->razerTimer >= 0.0f) {
							if (shooter->Shot[s]->razerTimer >= 0.45f) {
								targetJet[t]->health -= shooter->Shot[s]->Attack;
								hitedEffect(targetJet[t]);
							}
						}
						else if (shooter->Shot[s]->atkedJet != targetJet[t]) {
							targetJet[t]->health -= shooter->Shot[s]->Attack;
							hitedEffect(targetJet[t]);
							shooter->Shot[s]->atkedJet = targetJet[t];
							if (shooter->Shot[s]->Attack > 1) {
								shooter->Shot[s]->Attack -= shooter->Shot[s]->kantuTeika;
							}
							else {
								shooter->Shot[s]->Attack = 1.0f;
							}
							if (shooter->Shot[s]->handle == shotGfx[BOOMERANG])animStart(targetJet[t]->circle.pos, zangekiAnim);
						}
					}
					else {
						targetJet[t]->health -= shooter->Shot[s]->Attack;
						hitedEffect(targetJet[t]);
						SAFE_DELETE(shooter->Shot[s]);
					}
					//playerの子機の処理
					if (targetJet[t]->maxhealth == -100) {
						for (int i = 0; i < PlayerJet::MAX_KOKI_SUU; i++) {
							if (player->koki[i] && targetJet[t] == player->koki[i]) {
								player->koki[i] = nullptr;
								for (int k = 0; k < MAX_SHOT_SUU; k++) {
									if (player->Shot[k] && player->Shot[k] == targetJet[t]) {
										SAFE_DELETE(player->Shot[k]);
										break;
									}
								}
								break;
							}
						}
						targetJet[t] = nullptr;
					}
				}
			}
			if (shooter->Shot[s] && shooter->Shot[s]->razerTimer >= 0.45f) {
				shooter->Shot[s]->razerTimer = 0.0f;
			}
		}
	}
	if (!count&&shooter->stat == Jet::DEAD)shooter->stat = Jet::SYOUKYO;
}

