#include <DxLib.h>
#include <math.h>
#include "gameinit.h"
#include "gameManager.h"
#include "scene/playScene.h"
#include "debug.h"
#include "keycon.h"
//‰æ–Ê“à{·•ª‚É‚¢‚½‚çtrue‚ð•Ô‚·

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager *gm = GameManager::getInstance();
	gameInit();
	//–ˆƒtƒŒ[ƒ€ˆ—
	bool debugsuitti=false;
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		gm->input->keyDownCheck();
		gm->input->mouseDownCheck();
		GetMousePoint(&gm->cursor->mouseX, &gm->mouseY);
		switch (gm->gameScene)
		{
		case GameManager::initScene:

			gm->gameScene = GameManager::playScene;
			break;
		case GameManager::titleScene:
			break;
		case GameManager::playScene:
			gm->scene_play->playUpdate();
			break;
		case GameManager::resultScene:
			break;
		default:
			break;
		}
		
		DrawRotaGraph(gm->cursor->mouseX, gm->mouseY, 1.0, 0, gm->img[7], true);
		if (gm->input->isKeyDownTrigger(KEY_INPUT_F3))debugsuitti=!debugsuitti;
		gm->debug->update(debugsuitti);
	}

	DxLib_End();
	return 0;
}