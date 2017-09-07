#include "gameManager.h"
#include "keycon.h"
#include "Debug.h"
#include "cursor.h"
#include "JetManager.h"
#include "TamaJet.h"
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
void GameManager::playInit() {
	SAFE_DELETE(scene_title);
	JetManager *jm = JetManager::getInstance();
	gameScene = play;
	scene_play = new playscene();
	//敵機作成
	for (int i = 0; i < 10; i++) {
		int n = (i + 1) % 7 + 1;
		jm->enemy[i] = new EnemyJet(winWidth - 50, GetRand(winHeight), 0, 64, GetRand(5) + 1, n, 0.3f + GetRand(5.0f) / 5.0f, jm->gfx[JetManager::ENEMY_JET_TYPE1], n, 50 + n);
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
	//enemyを全部デリート
	for (int i = 0; i < JetManager::MAX_ENEMY_SUU&&jm->enemy[i]; i++) {
		SAFE_DELETE(jm->enemy[i]);
	}
	
}
void  GameManager::resultInit(int kekka) {

	scene_result = new resultscene(kekka);
	gameScene = result;

}
void GameManager::resultEnd() {
	SAFE_DELETE(scene_result);
}