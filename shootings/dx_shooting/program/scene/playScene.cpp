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

	//�N���b�N�Œe����鏈��
	if (GetMouseInput()&MOUSE_INPUT_LEFT&&jm->player->atkTimer >= jm->player->AttackSpeed) {
		jm->player->shotGen(gm->cursor->mouseX, gm->cursor->mouseY, 0xffffff, 5, 2, 10, jm->gfx[JetManager::ZIKI_MISS]);
	}
	//�G�`��
	int enemyCount = 0;
	for (int s = 0; s < JetManager::MAX_ENEMY_SUU; s++) {
		if (!jm->enemy[s])continue;
		if (jm->enemy[s]->stat==Jet::TAIKI||jm->enemy[s]->stat==Jet::LIVE) {
			enemyCount++;
			//�G���ہ[��
			if (jm->enemy[s]->stat == Jet::TAIKI&&playTimer >= jm->enemy[s]->spawnTimer) {
				jm->enemy[s]->stat = Jet::LIVE;
			}
		}

		if (jm->enemy[s]->stat == Jet::LIVE) {
			//�G�̒e���B
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


	//�G�l�~�[���^�[�Q�b�g�ɓ����
	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		if (jm->enemy[i] && jm->enemy[i]->stat == Jet::LIVE)jm->inToTarget(jm->enemy[i]);
	}
	jm->damagesSyori(jm->player);
	jm->clearTarget();
	//�G�l�~�[���^�[�Q�b�g����J��

	//�v���C�����^�[�Q�b�g�ɓ����
	jm->inToTarget(jm->player);
	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		if (jm->enemy[i]) {
			jm->damagesSyori(jm->enemy[i]);
			//stat��syoukyo���ƃf���[�g
			if (jm->enemy[i]->health <= 0 && jm->enemy[i]->stat == Jet::LIVE) {
				jm->enemy[i]->stat = Jet::DEAD;
				jm->animStart(jm->enemy[i]->circle.pos, jm->bomAnim);
			}
			//�f�X�^�C�}�[��dead
			if (jm->enemy[i]->deathTimer <= playTimer) {
				jm->enemy[i]->stat = Jet::DEAD;
			}
			if (jm->enemy[i] && (jm->enemy[i]->stat == Jet::SYOUKYO)) {
				SAFE_DELETE(jm->enemy[i]);
			}
		}
	}
	jm->clearTarget();
	//�v���C���[���^�[�Q�b�g����J��
	if (!enemyCount)return VICTORY;
	if (jm->player->health <= 0) {
		return DEFEAT;
		//GAMEOVER����
	}
	return FIGHTING;

}