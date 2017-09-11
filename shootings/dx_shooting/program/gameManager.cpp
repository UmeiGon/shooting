#include "gameManager.h"
#include "keycon.h"
#include "Debug.h"
#include "cursor.h"
#include "JetManager.h"
#include "TamaJet.h"
#include "enemyJet.h"
#include "playerJet.h"
#include "scene/titleScene.h"
#include "scene/playScene.h"
#include "scene/resultScene.h"
#include "scene/menuScene.h"

GameManager* GameManager::getInstance() {
	static GameManager* instance = nullptr;
	if (instance == nullptr) {
		instance = new GameManager();
	}
	return instance;
}
GameManager::GameManager() {
	input = keymouseInput::getInstance();
	debug = Debug::getInstance();
	cursor = Cursor::getInstance();
	gameScene = init;
}
void GameManager::drawBar(float startX, float startY, float w, float h, int value, int maxValue, int col, int fillCol) {
	float H = (float)value / (float)maxValue;
	if (col == -1)col = GetColor(0, 255, 0);
	if (fillCol == -1)fillCol = GetColor(255, 0, 0);
	DrawBox(startX, startY, startX + w, startY + h, fillCol, true);
	DrawBox(startX, startY, startX + H *w, startY + h, col, true);

}
void GameManager::gameInit() {
	SetGraphMode(1920, 1080, 16, 60);

	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);//ウィンドウモード変更と初期化と裏画面設定
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	//背景グラフィック
	int a = LoadGraph("images/UI.png");
	backImg[CLOUD1_B] = LoadGraph("images/cloud1.png");
	backImg[MENU_B] = DerivationGraph(256, 160, 64, 64, a);
	//UIグラフィック
	UIImg[MIGI] = LoadGraph("images/migi.png");
	UIImg[HIDARI] = LoadGraph("images/hidari.png");
	UIImg[SHOTFLAME] = LoadGraph("images/waku01.png");
	UIImg[ROBO_ICON] = LoadGraph("images/RoboIcon.bmp");

	DeleteGraph(a);
	JetManager *jm = JetManager::getInstance();
	scene_title = new titlescene();
	cursor->graphInit();
	GetWindowSize(&winWidth, &winHeight);
	GetWindowSize(&battleWidth, &battleHeight);
	battleHeight -= UI_ZONE;
	jm->init();
}
void GameManager::playInit() {
	SAFE_DELETE(scene_title);
	JetManager *jm = JetManager::getInstance();
	gameScene = play;
	scene_play = new playscene();
	//敵機作成
	for (int i = 0; i < 10; i++) {
		int n = (i + 1) % 7 + 1;
		jm->enemy[i] = new EnemyJet(battleWidth - 50, GetRand(battleHeight), 0, 64, GetRand(5) + 1, 5, 0.3f + GetRand(5.0f) / 5.0f, jm->gfx[JetManager::ENEMY_JET_TYPE1], n, 50 + n);
		jm->enemy[i]->addHoudai(0, 0, 0, jm->gfx[JetManager::ENEMY_HOUDAI], 40.0f / (float)jm->getGraphYsize(jm->gfx[JetManager::ENEMY_HOUDAI]), MY_PI);
	}
	jm->player->playerInit();
}
void GameManager::menuInit() {
	gameScene = menu;
	scene_menu = new menuscene();
}
void GameManager::menuEnd() {
	SAFE_DELETE(scene_menu);
}
void GameManager::playEnd() {
	SAFE_DELETE(scene_play);
	JetManager *jm = JetManager::getInstance();
	jm->ultActive = -1;
	for (int i = 0; i < JetManager::MAX_ITEM_SUU; i++) {
		SAFE_DELETE(jm->item[i]);
	}
	//enemyを全部デリート
	for (int i = 0; i < JetManager::MAX_ENEMY_SUU; i++) {
		if (jm->enemy[i]) {
			SAFE_DELETE(jm->enemy[i]);
		}
	}
}
void  GameManager::resultInit(int kekka) {

	scene_result = new resultscene(kekka);
	gameScene = result;

}
void GameManager::resultEnd() {
	SAFE_DELETE(scene_result);
}