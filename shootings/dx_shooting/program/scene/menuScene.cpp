#include "menuScene.h"
#include "../gameManager.h"
#include "../inputBox.h"
#include "../playerJet.h"
#include "../jetManager.h"
#include "../rune.h"
void box(int x, int y, int wid, int hei) {
	GameManager* gm = GameManager::getInstance();
	DrawModiGraph(x, y, x + wid, y, x + wid, y + hei, x, y + hei, gm->UIImg[gm->SHOTFLAME], true);
}
bool menuscene::update() {
	GameManager* gm = GameManager::getInstance();
	JetManager* jm = JetManager::getInstance();
	if (gm->input->isKeyDownTrigger(KEY_INPUT_ESCAPE)) {
		return true;
	}
	//背景
	for (int x = 1; x < gm->winWidth; x += 64) {
		for (int y = 1; y < gm->winHeight; y += 64) {
			DrawGraph(x, y, gm->backImg[gm->MENU_B], false);
		}
	}
	int off = 220;
	int hei = 160;
	int wid = gm->winWidth - 220 * 2;
	box(220, gm->winHeight - 180, wid, hei);
	for (int i = 0; i < jm->player->MAX_RUNE_SUU; i++) {
		for (int y = 1; y <= 3; y++) {
			for (int x = 1; x <= 10; x++) {
				if (jm->player->runes[i]) {
					DrawRotaGraph(off + wid / 11 * x, (gm->winHeight - 180) + hei / 4 * y, 1.0f, 0, jm->itemGfx[(2 + jm->player->runes[i]->type)], true);
					break;
				}
			}
		}
	}

	for (int atype = 0; atype <= PlayerJet::ULT; atype++) {
		if (jm->player->nowShot[atype] == -1) continue;
		if (leftBox[atype]->update()) {
			//左を押したら実体のショットが出るまでマイナスを繰り返す。
			for (int k = jm->player->nowShot[atype] - 1; jm->player->nowShot[atype] != k; k--) {
				if (k < 0)k = PlayerJet::MAX_SHOT_TYPE - 1;

				if (jm->player->shotData[atype][k]) {
					jm->player->nowShot[atype] = (PlayerJet::pShotType)k;
					break;
				}
			}
		}
		if (rightBox[atype]->update()) {
			//右を押したら出るまでプラス		
			for (int k = jm->player->nowShot[atype] + 1; jm->player->nowShot[atype] != k; k++) {
				if (k > PlayerJet::MAX_SHOT_TYPE - 1)k = 0;
				if (jm->player->shotData[atype][k]) {
					jm->player->nowShot[atype] = (PlayerJet::pShotType)k;
					break;
				}
			}
		}
	}
	DrawRotaGraph(gm->winWidth / 2, wepy[0], 1.6f, 0, gm->UIImg[GameManager::ROBO_ICON], false);
	jm->shotIconDraw(wepx[0], wepy[0] - wepsize / 2, PlayerJet::MAIN, jm->player->nowShot[PlayerJet::MAIN], wepsize);
	jm->shotIconDraw(wepx[1], wepy[1] - wepsize / 2, PlayerJet::SUB, jm->player->nowShot[PlayerJet::SUB], wepsize);
	jm->shotIconDraw(wepx[2], wepy[2] - wepsize / 2, PlayerJet::ULT, jm->player->nowShot[PlayerJet::ULT], wepsize);

	return false;
}
menuscene::menuscene() {
	JetManager* jm = JetManager::getInstance();
	GameManager* gm = GameManager::getInstance();
	int size = 50;
	wepsize = 160;
	int wep = wepsize;
	int center = gm->winWidth / 2;
	int offset = (wep + size * 2) / 2;
	int aida = 350;
	int sx[3]{ center - aida - offset,center + aida - offset,center - offset };
	int sy[3]{ 600,600,300 };
	for (int i = 0; i < 3; i++) {
		wepx[i] = sx[i] + size;
		wepy[i] = sy[i];
	}
	leftBox[PlayerJet::MAIN] = new ClickBox(sx[0], sy[0], size, size, "", 0, 0, gm->UIImg[GameManager::HIDARI]);
	leftBox[PlayerJet::SUB] = new ClickBox(sx[1], sy[1], size, size, "", 0, 0, gm->UIImg[GameManager::HIDARI]);
	leftBox[PlayerJet::ULT] = new ClickBox(sx[2], sy[2], size, size, "", 0, 0, gm->UIImg[GameManager::HIDARI]);
	rightBox[PlayerJet::MAIN] = new ClickBox(sx[0] + wep + size, sy[0], size, size, "", 0, 0, gm->UIImg[GameManager::MIGI]);
	rightBox[PlayerJet::SUB] = new ClickBox(sx[1] + wep + size, sy[1], size, size, "", 0, 0, gm->UIImg[GameManager::MIGI]);
	rightBox[PlayerJet::ULT] = new ClickBox(sx[2] + wep + size, sy[2], size, size, "", 0, 0, gm->UIImg[GameManager::MIGI]);
}