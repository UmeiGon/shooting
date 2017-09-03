#include "Dxlib.h"
#include "gameManager.h"
#include "gameInit.h"
#include "jetManager.h"
#include "TamaJet.h"
#include "scene/playScene.h"
#include "scene/titleScene.h"
#include "keycon.h"
#include "cursor.h"
void gameInit() {
	SetGraphMode(1500, 800, 16, 60);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);//ウィンドウモード変更と初期化と裏画面設定
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	GameManager *gm = GameManager::getInstance();
	JetManager *jm = JetManager::getInstance();
	gm->scene_title = new titlescene();
	gm->cursor->graphInit();
	LoadDivGraph("images/bakuhatsu01.png", 12, 3, 4, 64, 64, jm->bomAnim);
	GetWindowSize(&gm->winWidth, &gm->winHeight);
	jm->init();
	
	
	//自機作成

}