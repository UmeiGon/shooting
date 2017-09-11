#include "menuScene.h"
#include "../gameManager.h"
#include "../inputBox.h"
#include "../playerJet.h"
#include "../jetManager.h"
bool menuscene::update() {
	GameManager* gm = GameManager::getInstance();
	JetManager* jm = JetManager::getInstance();
	if (gm->input->isKeyDownTrigger(KEY_INPUT_ESCAPE)) {
		return true;
	}
	for (int x = 1; x < gm->winWidth; x += 64) {
		for (int y = 1; y < gm->winHeight; y += 64) {
			DrawGraph(x, y, gm->backImg[gm->MENU_B], false);
		}
	}
	for (int atype = 0; atype <= PlayerJet::ULT; atype++) {
		if (jm->player->nowShot[atype] == -1) continue;
		if (leftBox[atype]->update()) {
			//��������������̂̃V���b�g���o��܂Ń}�C�i�X���J��Ԃ��B
			for (int k = jm->player->nowShot[atype] - 1; jm->player->nowShot[atype] != k; k--) {
				if (k < 0)k = PlayerJet::MAX_SHOT_TYPE - 1;

				if (jm->player->shotData[atype][k]) {
					jm->player->nowShot[atype] = (PlayerJet::pShotType)k;
					break;
				}
			}
		}
		if (rightBox[atype]->update()) {
			//�E����������o��܂Ńv���X		
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
	jm->shotIconDraw(wepx[0], wepy[0]-wepsize/2, PlayerJet::MAIN, jm->player->nowShot[PlayerJet::MAIN], wepsize);
	jm->shotIconDraw(wepx[1], wepy[1]-wepsize/2, PlayerJet::SUB, jm->player->nowShot[PlayerJet::SUB],wepsize);
	jm->shotIconDraw(wepx[2], wepy[2]-wepsize/2, PlayerJet::ULT, jm->player->nowShot[PlayerJet::ULT],wepsize);
	
	return false;
}
menuscene::menuscene() {
	JetManager* jm = JetManager::getInstance();
	GameManager* gm = GameManager::getInstance();
	int size = 50;
	wepsize=160;
	int wep = wepsize;
	int center = gm->winWidth/2;
	int offset = (wep + size * 2)/2;
	int aida = 350;
	int sx[3]{center-aida-offset,center+aida - offset,center - offset };
	int sy[3]{600,600,300};
	for (int i = 0; i < 3; i++) {
		wepx[i] = sx[i]+size;
		wepy[i] = sy[i];
	}
	leftBox[PlayerJet::MAIN] = new ClickBox(sx[0],sy[0], size,size,"", 0, 0, gm->UIImg[GameManager::HIDARI]);
	leftBox[PlayerJet::SUB] = new ClickBox(sx[1], sy[1] , size, size, "", 0, 0, gm->UIImg[GameManager::HIDARI]);
	leftBox[PlayerJet::ULT] = new ClickBox(sx[2],sy[2], size, size, "", 0, 0, gm->UIImg[GameManager::HIDARI]);
	rightBox[PlayerJet::MAIN] = new ClickBox(sx[0]+wep+size,sy[0], size, size, "", 0, 0, gm->UIImg[GameManager::MIGI]);
	rightBox[PlayerJet::SUB] = new ClickBox(sx[1]+wep+size,sy[1],size, size, "", 0, 0, gm->UIImg[GameManager::MIGI]);
	rightBox[PlayerJet::ULT] = new ClickBox(sx[2]+wep+size,sy[2], size, size, "", 0, 0, gm->UIImg[GameManager::MIGI]);
}