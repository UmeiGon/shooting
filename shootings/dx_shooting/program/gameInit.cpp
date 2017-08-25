#include "Dxlib.h"
#include "gameManager.h"
#include "gameInit.h"
#include "jetManager.h"
#include "TamaJet.h"
#include "scene/playScene.h"
#include "keycon.h"
void gameInit() {
	SetGraphMode(1000, 800, 16, 60);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);//ウィンドウモード変更と初期化と裏画面設定
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	GameManager *gm = GameManager::getInstance();
	JetManager *jm = JetManager::getInstance();
	gm->scene_play = new playscene();
	gm->img[0] = LoadGraph("images/title.jpg", false);
	gm->img[1] = LoadGraph("images/mons1.png", false);
	gm->img[2] = LoadGraph("images/mons2.png", false);
	gm->img[3] = LoadGraph("images/mons3.png", false);
	gm->img[4] = LoadGraph("images/mons4.png", false);
	gm->img[5] = LoadGraph("images/mons5.png", false);
	gm->img[6] = LoadGraph("images/mons6.png", false);
	gm->img[7] = LoadGraph("images/clickCursor.png", false);
	GetWindowSize(&gm->winWidth, &gm->winHeight);
	//敵機作成
	for (int i = 0; i < 10; i++) {
		int n = (i + 1) % 7 + 1;
		jm->enemy[i] = new EnemyJet(gm->winWidth - 50, GetRand(gm->winHeight), 0, 64, GetRand(5) + 1, n, 0.3f + GetRand(5.0f) / 5.0f, gm->img[n]);
	}
	//自機作成
	jm->player = new PlayerJet(gm->winWidth / 2, gm->winHeight / 2, -MY_PI / 2, 20, 10, 3, 0.2, LoadGraph("images/ziki.png", false));
}