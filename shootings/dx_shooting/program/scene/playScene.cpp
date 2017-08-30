#include "playScene.h"
#include "Dxlib.h"
#include "../debug.h"
#include "../gameManager.h"
#include "../jetManager.h"
#include "../TamaJet.h"
#include "../keycon.h"
#include "../cursor.h"
playscene::playscene() {
	jm = JetManager::getInstance();
	playTimer = 0;
}
int playscene::update() {
	GameManager* gm = GameManager::getInstance();
	playTimer += gm->debug->dTime;
	DrawFormatString(0, 900, 0xffffff, "%f", playTimer);

	jm->player->pMove();
	jm->player->drawJet();

	//クリックで弾を作る処理
	if (GetMouseInput()&MOUSE_INPUT_LEFT&&jm->player->atkTimer >= jm->player->AttackSpeed) {
		jm->player->shotGen(gm->cursor->mouseX, gm->cursor->mouseY, 0xffffff, 5, 2, 10, jm->gfx[JetManager::ZIKI_MISS]);
	}
	//敵描画
	int enemyCount = 0;
	for (int s = 0; s < JetManager::MAX_ENEMY_SUU; s++) {
		if (!jm->enemy[s])continue;
		if (jm->enemy[s]->stat==Jet::TAIKI||jm->enemy[s]->stat==Jet::LIVE) {
			enemyCount++;
			//敵すぽーん
			if (jm->enemy[s]->stat == Jet::TAIKI&&playTimer >= jm->enemy[s]->spawnTimer) {
				jm->enemy[s]->stat = Jet::LIVE;
			}
		}

		if (jm->enemy[s]->stat == Jet::LIVE) {
			//敵の弾作る。
			if (jm->enemy[s]->AttackSpeed <= jm->enemy[s]->atkTimer) {
				jm->enemy[s]->shotGen(jm->player->circle.pos.x, jm->player->circle.pos.y, GetColor(2, 200, 2), (float)(GetRand(9) + 3) / 3.0f, 1, 14, jm->gfx[JetManager::ENEMY_MISS], MY_PI);
			}
			/*if (jm->enemy[s]->deathTimer&& jm->enemy[s]->Timer >= jm->enemy[s]->deathTimer) {
				SAFE_DELETE(jm->enemy[s]);
			}*/
			
			jm->enemy[s]->eneMove();
			jm->enemy[s]->drawJet();
			if (jm->hitHantei(jm->enemy[s], jm->player)) {
				jm->player->health--;
			}
		}

	}


	//エネミーをターゲットに入れる
	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		if (jm->enemy[i] && jm->enemy[i]->stat == Jet::LIVE)jm->inToTarget(jm->enemy[i]);
	}
	jm->damagesSyori(jm->player);
	jm->clearTarget();
	//エネミーをターゲットから開放

	//プレイヤをターゲットに入れる
	jm->inToTarget(jm->player);
	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		if (jm->enemy[i]) {
			jm->damagesSyori(jm->enemy[i]);
			//statがsyoukyoだとデリート
			if (jm->enemy[i]->health <= 0 && jm->enemy[i]->stat == Jet::LIVE) {
				jm->enemy[i]->stat = Jet::DEAD;
				jm->animStart(jm->enemy[i]->circle.pos, jm->bomAnim);
			}
			//デスタイマーでdead
			if (jm->enemy[i]->deathTimer <= playTimer) {
				jm->enemy[i]->stat = Jet::DEAD;
			}
			if (jm->enemy[i] && (jm->enemy[i]->stat == Jet::SYOUKYO)) {
				SAFE_DELETE(jm->enemy[i]);
			}
		}
	}
	jm->clearTarget();
	//プレイヤーをターゲットから開放
	if (!enemyCount)return VICTORY;
	if (jm->player->health <= 0) {
		return DEFEAT;
		//GAMEOVER処理
	}
	return FIGHTING;

}