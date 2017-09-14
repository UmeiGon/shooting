#include "jetManager.h"
#include "gameManager.h"
#include "playerJet.h"
#include "enemyJet.h"
#include "TamaJet.h"
#include "debug.h"
#include "Animation.h"
#include "Dxlib.h"
#include "scene/playScene.h"
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
	gfx[ENEMY_JET_TYPE2] = LoadGraph("images/enemy2.png");
	gfx[ENEMY_JET_TYPE3] = LoadGraph("images/enemy3.png");
	gfx[ENEMY_JET_TYPE4] = LoadGraph("images/enemy4.png");
	gfx[ENEMY_JET_TYPE5] = LoadGraph("images/enemy5.png");
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
	shotGfx[KI] = LoadGraph("images/blueshot.png");
	shotGfx[RICE] = DerivationGraph(66, 150, 12, 5, c);
	//アイテムのグラ
	itemGfx[IRON] = LoadGraph("images/iron.png");
	itemGfx[ENERGY] = LoadGraph("images/energy.png");
	itemGfx[HOUNETU_GFX] = LoadGraph("images/houneturune.png");
	itemGfx[CD_GFX] = LoadGraph("images/cdrune.png");
	itemGfx[SPD_GFX] = LoadGraph("images/speedrune.png");
	itemGfx[ATK_GFX] = LoadGraph("images/attackrune.png");

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
	memset(item, 0, sizeof(item));
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
	drawsize = (float)fsize / ((float)s + 12.0f);
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
			animStart(t2k::vec3(gm->battleWidth / 2, gm->battleHeight - 250, 0), ultbom1Anim, 3.0f);
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
			animStart(t2k::vec3(gm->battleWidth / 2, gm->battleHeight - 250 - animnum*300.0f, 0), ultbom2Anim, 3.0f);
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
int JetManager::enemySousa() {
	GameManager *gm = GameManager::getInstance();
	int enemyCount = 0;
	for (int s = 0; s < MAX_ENEMY_SUU; s++) {
		if (!enemy[s])continue;
		if (enemy[s]->stat == Jet::TAIKI || enemy[s]->stat == Jet::LIVE) {
			enemyCount++;
			//敵すぽーん
			if (enemy[s]->stat == Jet::TAIKI&& gm->scene_play->playTimer >= enemy[s]->spawnTimer) {
				enemy[s]->stat = Jet::LIVE;
			}
		}

		if (enemy[s]->stat == Jet::LIVE) {
			//敵の弾作る。
			if (enemy[s]->AttackSpeed <= enemy[s]->atkTimer) {
				switch (enemy[s]->shotType)
				{
				case 0:
					enemy[s]->shotGen(EnemyJet::MISSILE1, true, player->circle.pos.x, player->circle.pos.y);
					break;
				case 1:
					enemy[s]->shotGen(EnemyJet::RICE, false, enemy[s]->circle.pos.x + cos(enemy[s]->angle - 0.2f), enemy[s]->circle.pos.y + sin(enemy[s]->angle - 0.2f));
					enemy[s]->shotGen(EnemyJet::RICE, false, enemy[s]->circle.pos.x + cos(enemy[s]->angle), enemy[s]->circle.pos.y + sin(enemy[s]->angle));
					enemy[s]->shotGen(EnemyJet::RICE, false, enemy[s]->circle.pos.x + cos(enemy[s]->angle + 0.2f), enemy[s]->circle.pos.y + sin(enemy[s]->angle + 0.2f));
					break;
				case 2:
					enemy[s]->shotGen(EnemyJet::KI,false, player->circle.pos.x, player->circle.pos.y);

					break;
				case 3:
					for (int i = 0; i < 8;i++) {
						enemy[s]->shotGen(EnemyJet::RICE, false, enemy[s]->circle.pos.x + cos(enemy[s]->angle+MY_PI/4*i), enemy[s]->circle.pos.y + sin(enemy[s]->angle + MY_PI / 4 * i));
					}
					break;
				default:
					break;
				}
				enemy[s]->atkTimer = 0;
			}
			/*if (jm->enemy[s]->deathTimer&& jm->enemy[s]->Timer >= jm->enemy[s]->deathTimer) {
			SAFE_DELETE(jm->enemy[s]);
			}*/

			enemy[s]->eneMove();
			enemy[s]->drawJet();
			if (hitHantei(enemy[s], player)) {
				player->health--;
			}
		}
	}
	return enemyCount;
}
void JetManager::genStage0() {
	float randtime = (float)(GetRand(19)+1)/5;
	for (int i = 0; i < MAX_ENEMY_SUU; i++) {
		enemy[i] = genEnemy(GetRand(1),i+randtime);
	}
}
void JetManager::genStage1() {
	float randtime = (float)(GetRand(19) + 1) / 5;
	for (int i = 0; i < MAX_ENEMY_SUU; i++) {
		enemy[i] = genEnemy(GetRand(2), i + randtime);
	}
}
void JetManager::genStage2() {
	float randtime = (float)(GetRand(19) + 1) / 5;
	for (int i = 0; i < MAX_ENEMY_SUU; i++) {
		enemy[i] = genEnemy(GetRand(3), i + randtime);
	}
}
void JetManager::genStage3() {
	float randtime = (float)(GetRand(19) + 1) / 5;
	for (int i = 0; i < MAX_ENEMY_SUU; i++) {
		if (i == MAX_ENEMY_SUU - 1) {
			enemy[i] = genEnemy(4,i);
			break;
		}
		enemy[i] = genEnemy(GetRand(3), i + randtime);
	}
}
EnemyJet* JetManager::genEnemy(int ty,float times) {
	GameManager* gm = GameManager::getInstance();
	EnemyJet *ene;
	int rand = GetRand(2) + 1;
	switch (ty)
	{
		
	case 0:
		ene = new EnemyJet(gm->battleWidth - 50 * rand, GetRand(gm->battleHeight-100)+80, 0, 64, GetRand(5) + 1, 10.0f, 1.8f, gfx[JetManager::ENEMY_JET_TYPE1], 1, 0,times,times+10.0f);
		ene->addHoudai(0, 0, 0,gfx[JetManager::ENEMY_HOUDAI], 40.0f / (float)getGraphYsize(gfx[JetManager::ENEMY_HOUDAI]), MY_PI);
		break;
	case 1:
		ene = new EnemyJet(gm->battleWidth -50*rand , gm->battleHeight, 0, 64, GetRand(5) + 1, 15.0f, 2.5f, gfx[JetManager::ENEMY_JET_TYPE3], 3, 2, times, times + 10.0f);
		break;
	case 2:
		ene = new EnemyJet(gm->battleWidth - 50 * rand, GetRand(gm->battleHeight - 100) + 80, 0, 30, GetRand(5) + 1, 8.0f, 1.2f, gfx[JetManager::ENEMY_JET_TYPE4], 1, 1, times, times + 10.0f);
		break;
	case 3:
		ene = new EnemyJet(gm->battleWidth - 50 * rand, GetRand(gm->battleHeight - 100) + 80, 0, 25, GetRand(5) + 1, 4.0f, 1.5f, gfx[JetManager::ENEMY_JET_TYPE5], 1, 1, times, times + 10.0f);
		break;
	case 4:
		ene = new EnemyJet(gm->battleWidth/2, GetRand(gm->battleHeight),0, 450,GetRand(5) + 1, 255.0f, 0.5f, gfx[JetManager::ENEMY_JET_TYPE2],4,3, times, times + 9999.0f);
		break;
	default:
		break;
	}
	return ene;
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

