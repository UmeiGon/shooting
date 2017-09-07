#include "menuScene.h"
#include "../gameManager.h"
#include "../keycon.h"
#include "../inputBox.h"
#include "../TamaJet.h"
#include "../jetManager.h"
bool menuscene::update() {
	GameManager* gm = GameManager::getInstance();
	JetManager* jm = JetManager::getInstance();
	if (gm->input->isKeyDownTrigger(KEY_INPUT_ESCAPE)) {
		return true;
	}
	for ( int atype = 0; atype < PlayerJet::ULT; atype++) {
		if (leftBox[atype]->update()) {
			//左を押したら実体のショットが出るまでマイナスを繰り返す。
			for (int k = jm->player->nowShot[atype]-1; k >= 0; k--) {
				if (jm->player->nowShot[atype] == -1) break;
				if (jm->player->shotData[atype][k]) {
					jm->player->nowShot[atype] = (PlayerJet::pShotType)k;
					break;
				}
				if (k == 0) {
					k = PlayerJet::MAX_SHOT_TYPE;
				}
			}
		}
		if (rightBox[atype]->update()) {
			//右を押したら出るまでプラス
			for (int k = jm->player->nowShot[atype] + 1; k <= PlayerJet::MAX_SHOT_TYPE; k++) {
				if (jm->player->nowShot[atype] == -1) break;
				if (jm->player->shotData[atype][k]) {
					jm->player->nowShot[atype] = (PlayerJet::pShotType)k;
					break;
				}
				if (k==PlayerJet::MAX_SHOT_TYPE) {
					k = 0;
				}
			}
		}
	}
	jm->shotIconDraw(100, 50, PlayerJet::MAIN,jm->player->nowShot[PlayerJet::MAIN], 50);
	jm->shotIconDraw(100,100, PlayerJet::SUB,jm->player->nowShot[PlayerJet::SUB], 50);
	jm->shotIconDraw(100, 150,PlayerJet::ULT, jm->player->nowShot[PlayerJet::ULT], 50);

	return false;
}
menuscene::menuscene() {
	JetManager* jm = JetManager::getInstance();
	leftBox[PlayerJet::MAIN] = new ClickBox(50, 50, 40, 40, "", 0, 0, jm->UIgfx[JetManager::HIDARI]);
	leftBox[PlayerJet::SUB] = new ClickBox(50, 100, 40, 40, "", 0, 0, jm->UIgfx[JetManager::HIDARI]);
	leftBox[PlayerJet::ULT] = new ClickBox(50, 150, 40, 40, "", 0, 0, jm->UIgfx[JetManager::HIDARI]);
	rightBox[PlayerJet::MAIN] = new ClickBox(200, 50, 40, 40, "", 0, 0, jm->UIgfx[JetManager::MIGI]);
	rightBox[PlayerJet::SUB] = new ClickBox(200, 100, 40, 40, "", 0, 0, jm->UIgfx[JetManager::MIGI]);
	rightBox[PlayerJet::ULT] = new ClickBox(200, 150, 40, 40, "", 0, 0, jm->UIgfx[JetManager::MIGI]);
}