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
	SetGraphMode(1900, 1000, 16, 60);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);//ウィンドウモード変更と初期化と裏画面設定
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	GameManager *gm = GameManager::getInstance();
	JetManager *jm = JetManager::getInstance();
	gm->scene_title = new titlescene();
	gm->cursor->graphInit();
	//jetmanagerのグラフィック初期化
	jm->gfx[JetManager::ZIKI_ZENTAI] = LoadGraph("images/player1.png");
	jm->gfx[JetManager::FIRE] = LoadGraph("images/fire.png");
	jm->gfx[JetManager::ZIKI_MISS] = DerivationGraph(0, 24, 18, 5, jm->gfx[JetManager::ZIKI_ZENTAI]);
	jm->gfx[JetManager::ZIKI_BEAM] = DerivationGraph(19, 25, 28, 3, jm->gfx[JetManager::ZIKI_ZENTAI]);
	jm->gfx[JetManager::ZIKI_JET] = DerivationGraph(0, 0, 51, 23, jm->gfx[JetManager::ZIKI_ZENTAI]);
	jm->gfx[JetManager::ENEMY_ZENTAI] = LoadGraph("images/enemy1.PNG");
	jm->gfx[JetManager::ENEMY_HOUDAI] = DerivationGraph(313, 272, 77, 54, jm->gfx[JetManager::ENEMY_ZENTAI]);
	jm->gfx[JetManager::ENEMY_JET_TYPE1] = DerivationGraph(163, 72, 108, 42, jm->gfx[JetManager::ENEMY_ZENTAI]);
	jm->gfx[JetManager::ENEMY_MISS] = DerivationGraph(50, 130, 47, 17, jm->gfx[JetManager::ENEMY_ZENTAI]);
	LoadDivGraph("images/bakuhatsu01.png", 12, 3, 4, 64, 64, jm->bomAnim);
	GetWindowSize(&gm->winWidth, &gm->winHeight);
}