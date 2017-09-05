#include "playScene.h"
#include "Dxlib.h"
#include "../debug.h"
#include "../gameManager.h"
#include "../jetManager.h"
#include "../TamaJet.h"
#include "../keycon.h"
#include "../cursor.h"
#include <math.h>
playscene::playscene() {
	jm = JetManager::getInstance();
	playTimer = 0;
}
int playscene::update() {
	GameManager* gm = GameManager::getInstance();
	playTimer += gm->debug->dTime;
	DrawFormatString(0, 900, 0xffffff, "%f", playTimer);
	static float at;
	if (gm->input->isKeyDownTrigger(KEY_INPUT_Y)) {
		at = atan2(gm->cursor->mouseX-jm->player->circle.pos.x , gm->cursor->mouseY-jm->player->circle.pos.y );
	}
	DrawFormatString(60, 0, 0xffffff, "%f", at);
	jm->player->update();
	if (gm->input->isKeyDownTrigger(KEY_INPUT_1)) {
		jm->player->nowShot[PlayerJet::MAIN] = PlayerJet::MAIN_FIRE;
	}
	if (gm->input->isKeyDownTrigger(KEY_INPUT_2)) {
		jm->player->nowShot[PlayerJet::MAIN] = PlayerJet::MAIN_BEAM;
	}
	if (gm->input->isKeyDownTrigger(KEY_INPUT_3)) {
		jm->player->nowShot[PlayerJet::SUB] = PlayerJet::SUB_BOOMERANG;
	}
	//敵描画
	int enemyCount = 0;
	t2k::vec3 move(800, 300, 0);
	for (int i = 0; i < 100; i++) {
		t2k::vec3 a=t2k::vec3BezierSpline(t2k::vec3(0, 0, 0), t2k::vec3(600,500,0),t2k::vec3(700,700,0), move, 0.01f*i);
		DrawPixel(a.x, a.y, 0xffffff);
	}

	for (int s = 0; s < JetManager::MAX_ENEMY_SUU; s++) {
		if (!jm->enemy[s])continue;
		if (jm->enemy[s]->stat == Jet::TAIKI || jm->enemy[s]->stat == Jet::LIVE) {
			enemyCount++;
			//敵すぽーん
			if (jm->enemy[s]->stat == Jet::TAIKI&&playTimer >= jm->enemy[s]->spawnTimer) {
				jm->enemy[s]->stat = Jet::LIVE;
			}
		}

		if (jm->enemy[s]->stat == Jet::LIVE) {
			//敵の弾作る。
			/*if (jm->enemy[s]->AttackSpeed <= jm->enemy[s]->atkTimer) {
				jm->enemy[s]->shotGen(EnemyJet::FIRE,true,jm->player->circle.pos.x, jm->player->circle.pos.y);
				jm->enemy[s]->atkTimer = 0;
			}*/
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
			if (jm->enemy[i]->deathTime <= playTimer) {
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