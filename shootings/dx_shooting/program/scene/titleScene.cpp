#include "Dxlib.h"
#include "titleScene.h"
#include "../inputBox.h"
#include "../save.h"
titlescene::titlescene() {
	GameManager* gm = GameManager::getInstance();
	SetFontSize(30);
	sce = SAISYO;
	int gx, gy;
	for (int i = 0; i < GameManager::SAIDAI_DATA; i++) {
		GetGraphSize(gm->UIImg[GameManager::DATA1_GFX + i], &gx, &gy);
		FILE* fp;
		string datastr;
		switch (i)
		{
		case 0:
			fp = fopen("savedata1.dat", "rb");
			break;
		case 1:
			fp = fopen("savedata2.dat", "rb");
			break;
		case 2:
			fp = fopen("savedata3.dat", "rb");
			break;
		default:
			break;
		}
		if (fp&&fgetc(fp) == -1) {
			datastr = "NO_DATA";
			gm->saveflag[i] = false;
		}
		else {
			datastr = "CREATED";
			gm->saveflag[i] = true;
		}
		dataButton[i] = new ClickBox((gm->winWidth / 2) - gx / 2, gm->winHeight / 6 * (3 + i), gx, gy, datastr, -120, 0, gm->UIImg[GameManager::DATA1_GFX + i], true, gm->UIImg[GameManager::POWA]);
		deleteButton[i]=new ClickBox((gm->winWidth / 2) - gx / 2-65, gm->winHeight / 6 * (3 + i)+40, 65, 40, "íœ", 0, 0);
		deleteButton[i]->color=GetColor(255,0,0);
		fclose(fp);
	}
	GetGraphSize(gm->UIImg[GameManager::NEWGAME_GFX], &gx, &gy);
	newgameButton = new ClickBox((gm->winWidth / 2) - gx / 2, gm->winHeight / 6 * 3, gx, gy, "", 0, 0, gm->UIImg[GameManager::NEWGAME_GFX],true, gm->UIImg[GameManager::POWA]);
	GetGraphSize(gm->UIImg[GameManager::LOADGAME_GFX], &gx, &gy);
	loadgameButton = new ClickBox((gm->winWidth / 2) - gx / 2, gm->winHeight / 6 * 4, gx, gy, "", 0, 0, gm->UIImg[GameManager::LOADGAME_GFX], true, gm->UIImg[GameManager::POWA]);
	GetGraphSize(gm->UIImg[GameManager::EXIT_GFX], &gx, &gy);
	exitButton = new ClickBox((gm->winWidth / 2) - gx / 2, gm->winHeight / 6 * 5, gx, gy, "", 0, 0, gm->UIImg[GameManager::EXIT_GFX], true, gm->UIImg[GameManager::POWA]);
}
//Capsule* cap1=nullptr;
//Capsule* cap2=nullptr;
//t2k::vec3 *point;
//int r = 60;
titlescene::~titlescene() {
	SAFE_DELETE(newgameButton);
	SAFE_DELETE(loadgameButton);
	SAFE_DELETE(exitButton);
	for (int i = 0; i < 3; i++) {
		SAFE_DELETE(dataButton[i]);
		SAFE_DELETE(deleteButton[i]);
	}
	SetFontSize(16);
}
bool titlescene::update() {
	GameManager* gm = GameManager::getInstance();
	//”wŒi
	for (int x = 1; x < gm->winWidth; x += 64) {
		for (int y = 1; y < gm->winHeight; y += 64) {
			DrawGraph(x, y, gm->backImg[gm->MENU_B], false);
		}
	}
	switch (sce)
	{
	case titlescene::SAISYO:
		if (newgameButton->update()) {
			return true;
		}
		if (loadgameButton->update()) {
			sce = LOADSENTAKU;
		}
		if (exitButton->update()) {
			DxLib_End();
		}
		break;
	case titlescene::LOADSENTAKU:
		if (gm->input->isKeyDownTrigger(KEY_INPUT_ESCAPE)) {
			sce = SAISYO;
		}
		for (int i = 0; i < GameManager::SAIDAI_DATA; i++) {
			if (dataButton[i]->update()) {	
				if (gm->saveflag[i]) {
					gm->Load(i);
					return true;
				}
			}
			if (deleteButton[i]->update()) {
				gm->DeleteSave(i);
				dataButton[i]->text = "NO_DATA";
				gm->saveflag[i] = false;
			}
		}
		
		break;
	default:
		break;
	}

	return false;
}