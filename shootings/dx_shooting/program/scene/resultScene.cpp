#include "resultScene.h"
#include "../inputBox.h"
#include "playScene.h"
#include "../gameManager.h"
#include "../keycon.h"
resultscene::resultscene(int kekka) {
	retryButton = new ClickBox(300, 400, 200, 100, "Retry", 25, 25);
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
	if (gm->input->isKeyDownTrigger(KEY_INPUT_ESCAPE)) {
		return GO_MENU;
	}
	if (retryButton->update()) {
		return GO_PLAY;
	}
	return NONE;
}