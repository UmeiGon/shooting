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
}
int playscene::update() {
	GameManager* gm = GameManager::getInstance();

	gm->debug->objSuu = 0;
	jm->player->pMove();
	jm->player->drawJet();
	
	//ƒNƒŠƒbƒN‚Å’e‚ðì‚éˆ—
	if (GetMouseInput()&MOUSE_INPUT_LEFT&&jm->player->Timer >= jm->player->AttackSpeed) {
		jm->player->shotGen(gm->cursor->mouseX, gm->cursor->mouseY, 0xffffff, 5,2,10,jm->gfx[JetManager::ZIKI_MISS]);
	}
	//“G•`‰æ
	int enemyCount=0;
	for (int s = 0; s < JetManager::MAX_ENEMY_SUU; s++) {
		if (jm->enemy[s]) {
			enemyCount++;
			//“G‚Ì’eì‚éB
			if (jm->enemy[s]->AttackSpeed <= jm->enemy[s]->Timer) {
				jm->enemy[s]->shotGen(jm->player->pos.x, jm->player->pos.y, GetColor(2, 200, 2), (float)(GetRand(9) + 1) / 3.0f,1,14,jm->gfx[JetManager::ENEMY_MISS],MY_PI);
			}
			/*if (jm->enemy[s]->deathTimer&& jm->enemy[s]->Timer >= jm->enemy[s]->deathTimer) {
				SAFE_DELETE(jm->enemy[s]);
			}*/
			if (Tama::maruHantei(*jm->enemy[s], *jm->player)) {
				jm->player->health--;
			}
			jm->enemy[s]->eneMove();
			jm->enemy[s]->drawJet();
		}

	}
	//“–‚½‚è”»’è‚Æ’e•`‰æ



	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		jm->inToTarget(jm->enemy[i]);
	}
	jm->damagesSyori(jm->player);
	jm->clearTarget();

	jm->inToTarget(jm->player);
	
	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		jm->damagesSyori(jm->enemy[i]);	
		if (jm->enemy[i]&&jm->enemy[i]->health <= 0) {
			jm->animStart(jm->enemy[i]->pos,jm->bomAnim);
			SAFE_DELETE(jm->enemy[i]);
		}
	}
	jm->clearTarget();
	if (!enemyCount) { return VICTORY; }
	if (jm->player->health <= 0) {
		return DEFEAT;
		//GAMEOVERˆ—
	}
	return FIGHTING;

}