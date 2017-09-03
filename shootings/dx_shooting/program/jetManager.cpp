#include "jetManager.h"
#include "gameManager.h"
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
	//jet本体のグラ
	gfx[BOOST] = DerivationGraph(15, 98, 33, 28, b);
	gfx[ZIKI_ZENTAI] = LoadGraph("images/player1.png");
	gfx[ZIKI_JET] = LoadGraph("images/playerRobo.png");
	gfx[ENEMY_ZENTAI] = LoadGraph("images/enemy1.PNG");
	gfx[ENEMY_HOUDAI] = DerivationGraph(313, 272, 77, 54,  gfx[ENEMY_ZENTAI]);
	gfx[ENEMY_JET_TYPE1] = DerivationGraph(163, 72, 108, 42,  gfx[ENEMY_ZENTAI]);
	//アームのグラ
	armgfx[FIRE_WEAPON] = DerivationGraph(160, 106, 40, 13, a);
	armgfx[NORMAL_WEAPON] = DerivationGraph(160, 74, 32, 13, a);
	armgfx[BEAM_WEAPON] = DerivationGraph(160, 202, 48, 14, a);
	//弾のグラ
	shotGfx[FIRE1]= LoadGraph("images/fire.png");
	shotGfx[MISS1] = DerivationGraph(0, 24, 18, 5,  gfx[ZIKI_ZENTAI]);
	shotGfx[BEAM1] = DerivationGraph(19, 25, 28, 3,  gfx[ZIKI_ZENTAI]);
	shotGfx[MISS2] = DerivationGraph(50, 130, 47, 17,  gfx[ENEMY_ZENTAI]);
	shotGfx[BEAM2] = DerivationGraph(33, 131, 13, 4, c);
	shotGfx[BOOMERANG] = LoadGraph("images/Boomerang.png");
	//player生成
	GameManager* gm = GameManager::getInstance();
	player = new PlayerJet(gm->winWidth / 2, gm->winHeight / 2, 0, 50, 10, 3, 0.2, gfx[JetManager::ZIKI_JET]);
	player->addHoudai(-20, -15, 0, armgfx[JetManager::FIRE_WEAPON], 1, 0);
}
JetManager::JetManager() {

	memset(anims, 0, sizeof(anims));
	memset(enemy, 0, sizeof(enemy));
	memset(targetJet, 0, sizeof(targetJet));
	player = nullptr;
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
			DrawRotaGraph(anims[i]->pos.x, anims[i]->pos.y, 1.0, 0, anims[i]->img[(int)(anims[i]->timer / anims[i]->changeTime)], true);
		}
	}
}
void JetManager::animStart(t2k::vec3 Pos, int Img[]) {
	for (int i = 0; i < MAX_ANIM_SUU; i++) {
		if (!anims[i]) {
			anims[i] = new Animation(Pos, Img);
			break;
		}
	}
}
//判定only
void JetManager::inToTarget(Jet* target) {
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
void JetManager::damagesSyori(Jet* shooter) {
	if (!shooter)return;
	int count = 0;
	for (int s = 0; s < MAX_SHOT_SUU; s++) {
		if (shooter->Shot[s]) {
			count++;
			shooter->drawMoveShot(s);
			for (int t = 0; t < MAX_TARGET_SUU; t++) {
				if (shooter->Shot[s] && targetJet[t] && targetJet[t]->health > 0 && hitHantei(targetJet[t], shooter->Shot[s])) {
					targetJet[t]->health -= shooter->Shot[s]->Attack;
					SAFE_DELETE(shooter->Shot[s]);
					break;
				}
			}
		}

	}
	if (!count&&shooter->stat == Jet::DEAD)shooter->stat = Jet::SYOUKYO;
}

