#include <DxLib.h>
#include <math.h>
#include "gameinit.h"
#include "gameManager.h"
#include "scene/playScene.h"
#include "scene/titleScene.h"
#include "scene/resultScene.h"
#include "debug.h"
#include "keycon.h"
#include "cursor.h"
#include "jetmanager.h"
//‰æ–Ê“à{·•ª‚É‚¢‚½‚çtrue‚ð•Ô‚·

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager *gm = GameManager::getInstance();
	JetManager *jm = JetManager::getInstance();
	gameInit();
	//–ˆƒtƒŒ[ƒ€ˆ—
	bool debugsuitti = false;
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		gm->input->keyDownCheck();
		gm->input->mouseDownCheck();
		GetMousePoint(&gm->cursor->mouseX, &gm->cursor->mouseY);
		jm->animationUpdate();
		switch (gm->gameScene)
		{
		case GameManager::init:
			gm->gameScene = GameManager::title;
			break;
		case GameManager::title:
			if (gm->scene_title->update()) {
				SAFE_DELETE(gm->scene_title);
				gm->playInit();
			}
			break;
		case GameManager::play:
			switch (gm->scene_play->update())
			{
			case playscene::FIGHTING:
				break;
			case playscene::VICTORY:
				gm->resultInit(playscene::VICTORY);
				break;
			case playscene::DEFEAT:
				gm->resultInit(playscene::DEFEAT);
				break;
			}
			break;
		case GameManager::result:
			if (gm->scene_result->update()) {
				gm->playInit();
			}
			break;
		default:
			break;
		}

		DrawRotaGraph(gm->cursor->mouseX, gm->cursor->mouseY, 1.0, 0, gm->cursor->Image[Cursor::def], true);
		if (gm->input->isKeyDownTrigger(KEY_INPUT_F3))debugsuitti = !debugsuitti;
		gm->debug->update(debugsuitti);
	}

	DxLib_End();
	return 0;
}