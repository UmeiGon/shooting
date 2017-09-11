#include "playScene.h"
#include "Dxlib.h"
#include "../debug.h"
#include "../gameManager.h"
#include "../jetManager.h"
#include "../playerJet.h"
#include "../enemyJet.h"
#include "../capsule.h"
#include "../item.h"
#include "../rune.h"
#include <math.h>
playscene::playscene() {
	jm = JetManager::getInstance();
	playTimer = 0;
}
int playscene::update() {
	GameManager* gm = GameManager::getInstance();
	JetManager* jm = JetManager::getInstance();
	DrawModiGraph(0, 0, gm->battleWidth, 0, gm->battleWidth, gm->battleHeight, 0, gm->battleHeight, gm->backImg[GameManager::CLOUD1_B], false);
	playTimer += gm->debug->dTime;
	DrawFormatString(0, 900, 0xffffff, "%f", playTimer);
	jm->player->update();


	//デバッグ
	//if (gm->input->isKeyDownTrigger(KEY_INPUT_1)) {
	//	jm->player->nowShot[PlayerJet::MAIN] = PlayerJet::MAIN_FIRE;
	//}
	//if (gm->input->isKeyDownTrigger(KEY_INPUT_2)) {
	//	jm->player->nowShot[PlayerJet::MAIN] = PlayerJet::MAIN_BEAM;
	//}
	//if (gm->input->isKeyDownTrigger(KEY_INPUT_3)) {
	//	jm->player->nowShot[PlayerJet::SUB] = PlayerJet::SUB_BOOMERANG;
	//}

	jm->shotIconDraw(100, 50, PlayerJet::MAIN, jm->player->nowShot[PlayerJet::MAIN], 50);
	jm->shotIconDraw(100, 100, PlayerJet::SUB, jm->player->nowShot[PlayerJet::SUB], 50);
	jm->shotIconDraw(100, 150, PlayerJet::ULT, jm->player->nowShot[PlayerJet::ULT], 50);

	//敵描画
	int enemyCount = 0;
	t2k::vec3 move(800, 300, 0);
	//アイテム描画
	for (int i = 0; i < jm->MAX_ITEM_SUU; i++) {
		if (jm->item[i]) {
			jm->item[i]->ItemUpdate();
			if (jm->item[i]->circle.pos.x < 0 || jm->hitHantei(jm->item[i], jm->player)) {
				switch (jm->item[i]->tag)
				{
				case 0:
					jm->player->ironsuu++;
					break;
				case 1:
					jm->player->energyPoint++;
					break;
				case 2:
					for (int k = 0;k<PlayerJet::MAX_RUNE_SUU; k++) {
						if (!jm->player->runes[k]) {
							jm->player->runes[k] = new Rune(jm->item[i]->runeType,jm->item[i]->runekouka);
						}
					}
					break;
				default:
					break;
				}
				SAFE_DELETE(jm->item[i])
			};
		}

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
			if (jm->enemy[s]->AttackSpeed <= jm->enemy[s]->atkTimer) {
				jm->enemy[s]->shotGen(EnemyJet::FIRE, true, jm->player->circle.pos.x, jm->player->circle.pos.y);
				jm->enemy[s]->atkTimer = 0;
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
		if (jm->enemy[i] && jm->enemy[i]->stat == Jet::LIVE) {
			jm->inToTarget(jm->enemy[i]);
		}
	}
	jm->damagesSyori(jm->player);
	jm->clearTarget();
	//エネミーをターゲットから開放

	//プレイヤをターゲットに入れる
	jm->inToTarget(jm->player);
	//子機があれば子機もターゲットに
	for (int i = 0; i < PlayerJet::MAX_KOKI_SUU; i++) {
		if (jm->player->koki[i]) {
			jm->inToTarget(jm->player->koki[i]);
		}
	}
	jm->UltimateUpdate();
	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		if (jm->enemy[i]) {
			jm->damagesSyori(jm->enemy[i]);
			//healthが0以下でdeadにstatがsyoukyoだとデリート
			if (jm->enemy[i]->health <= 0 && jm->enemy[i]->stat == Jet::LIVE) {
				jm->enemy[i]->stat = Jet::DEAD;
				for (int k = 0; k < jm->MAX_ITEM_SUU; k++) {
					if (!jm->item[k]) {
						jm->item[k] = new Item(jm->enemy[i]->circle.pos.x, jm->enemy[i]->circle.pos.y, 30.0f, GetRand(2), 1.5f);
						break;
					}
				}
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