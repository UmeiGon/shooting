#include <DxLib.h>
#include <math.h>
#include "gameManager.h"
#include "scene/playScene.h"
#include "scene/titleScene.h"
#include "scene/resultScene.h"
#include "scene/menuScene.h"
#include "debug.h"
#include "keycon.h"
#include "cursor.h"
#include "jetmanager.h"
//‰æ–Ê“à{·•ª‚É‚¢‚½‚çtrue‚ð•Ô‚·

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager *gm = GameManager::getInstance();
	JetManager *jm = JetManager::getInstance();
	gm->gameInit();
	//–ˆƒtƒŒ[ƒ€ˆ—
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		gm->input->keyDownCheck();
		gm->input->mouseDownCheck();
		gm->cursor->cNum = Cursor::def;
		gm->debug->objSuu = 0;
		GetMousePoint(&gm->cursor->mouseX, &gm->cursor->mouseY);
		
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
				gm->playEnd();
				gm->resultInit(playscene::VICTORY);
				break;
			case playscene::DEFEAT:
				gm->playEnd();
				gm->resultInit(playscene::DEFEAT);
				break;
			}
			break;
		case GameManager::result:
			switch (gm->scene_result->update())
			{

			case resultscene::GO_MENU:
				gm->resultEnd();
				gm->menuInit();
				break;
			case resultscene::GO_PLAY:
				gm->resultEnd();
				gm->playInit();
				break;
			case resultscene::NONE:
				break;
			default:
				break;
			}
			break;
		case GameManager::menu:
			if (gm->scene_menu->update()) {
				gm->menuEnd();
				gm->playInit();
			}
			break;
		default:
			break;
		}
		jm->animationUpdate();
		gm->cursor->cursorDraw();
		if (gm->input->isKeyDownTrigger(KEY_INPUT_F3))gm->debug->showDebug = !gm->debug->showDebug;
		gm->debug->update();
	}

	DxLib_End();
	return 0;
}