#include "playScene.h"
#include "Dxlib.h"
#include "../debug.h"
#include "../gameManager.h"
#include "../jetManager.h"
#include "../TamaJet.h"
#include "../keycon.h"
playscene::playscene() {
	jm = JetManager::getInstance();
}
void playscene::playUpdate() {
	GameManager* gm = GameManager::getInstance();
	


	gm->debug->objSuu = 0;
	DrawGraph(0, 0, gm->img[0], false);
	jm->player->pMove();
	jm->player->drawJet();
	
	//ƒNƒŠƒbƒN‚Å’e‚ðì‚éˆ—
	if (GetMouseInput()&MOUSE_INPUT_LEFT&&jm->player->Timer >= jm->player->AttackSpeed) {
		jm->player->shotGen(gm->cursor->mouseX, gm->mouseY, 0xffffff, 1.5f);
	}
	//“G•`‰æ
	for (int s = 0; s < JetManager::MAX_ENEMY_SUU; s++) {
		if (jm->enemy[s]) {
			//“G‚Ì’eì‚éB
			if (jm->enemy[s]->AttackSpeed <= jm->enemy[s]->Timer) {
				jm->enemy[s]->shotGen(jm->player->pos.x, jm->player->pos.y, GetColor(2, 200, 2), (float)(GetRand(9) + 1) / 10.0f);
			}
			/*if (jm->enemy[s]->deathTimer&& jm->enemy[s]->Timer >= jm->enemy[s]->deathTimer) {
				SAFE_DELETE(jm->enemy[s]);
			}*/
			if (Tama::maruHantei(*jm->enemy[s], *jm->player)) {
				SAFE_DELETE(jm->player);
				DxLib_End();
			}
			jm->enemy[s]->eneMove();
			jm->enemy[s]->drawJet();
		}

	}
	//“–‚½‚è”»’è‚Æ’e•`‰æ



	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		jm->inToTarget(jm->enemy[i]);
	}
	jm->test2(jm->player);
	jm->clearTarget();

	jm->inToTarget(jm->player);
	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		jm->test2(jm->enemy[i]);
		if (jm->enemy[i]&&jm->enemy[i]->health <= 0) {
			SAFE_DELETE(jm->enemy[i]);
		}
	}
	jm->clearTarget();
	if (jm->player->health <= 0) {
		//GAMEOVERˆ—
	}

}