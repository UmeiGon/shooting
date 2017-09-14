#include "resultScene.h"
#include "../inputBox.h"
#include "playScene.h"
#include "../gameManager.h"
#include "../keycon.h"
resultscene::resultscene(int kekka) {
	retryButton = new ClickBox(600, 500, 600, 250, "Retry", 25, 25);
	retryButton->color = GetColor(0, 0, 0);
	switch (kekka)
	{
	default:
		break;
	case playscene::VICTORY:
		retryButton->text2 = "Ÿ‚¿I";
		break;
	case playscene::DEFEAT:
		retryButton->text2 = "•‰‚¯";
		break;
	}
}
resultscene::go resultscene::update() {
	GameManager* gm = GameManager::getInstance();
	//”wŒi
	for (int x = 1; x < gm->winWidth; x += 64) {
		for (int y = 1; y < gm->winHeight; y += 64) {
			DrawGraph(x, y, gm->backImg[gm->MENU_B], false);
		}
	}
	SetFontSize(50);
	if (gm->input->isKeyDownTrigger(KEY_INPUT_ESCAPE)) {
		SetFontSize(16);
		return GO_MENU;
	}
	if (retryButton->update()) {
		SetFontSize(16);
		return GO_PLAY;
	}
	SetFontSize(16);
	return NONE;
}