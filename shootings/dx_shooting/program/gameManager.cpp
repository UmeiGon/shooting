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
#include "save.h"


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
string GameManager::runeStr(int t) {
	switch (t)
	{
	case JetManager::HOUNETU_RUNE:
		return "放熱";
		break;
	case JetManager::CD_RUNE:
		return "装填速度";
		break;
	case JetManager::SPD_RUNE:
		return "機体速度";
		break;
	case JetManager::ATK_RUNE:
		return "攻撃";
		break;
	default:
		break;
	}
}
void GameManager::Save(int num) {
	saved = new saveData;
	saved->save(num + 1);
	SAFE_DELETE(saved);
}
void GameManager::Load(int num) {
	saved = new saveData;
	saved->load(num + 1);
	SAFE_DELETE(saved);
}

void GameManager::DeleteSave(int num) {
	FILE* fp;
	switch (num + 1)
	{
	case 1:
		fp = fopen("savedata1.dat", "wb");
		break;
	case 2:
		fp = fopen("savedata2.dat", "wb");
		break;
	case 3:
		fp = fopen("savedata3.dat", "wb");
		break;
	default:
		break;
	}
	fclose(fp);

}
void GameManager::drawBar(float startX, float startY, float w, float h, int value, int maxValue, int col, int fillCol) {
	float H = (float)value / (float)maxValue;
	if (col == -1)col = GetColor(0, 255, 0);
	if (fillCol == -1)fillCol = GetColor(255, 0, 0);
	DrawBox(startX, startY, startX + w, startY + h, fillCol, true);
	DrawBox(startX, startY, startX + H *w, startY + h, col, true);

}
//titleinitもここでやってしまう
void GameManager::gameInit() {
	SetGraphMode(1920, 1080, 16, 60);

	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);//ウィンドウモード変更と初期化と裏画面設定
	SetDrawMode(DX_DRAWMODE_BILINEAR);


	//背景グラフィック
	int a = LoadGraph("images/UI.png");
	int b = LoadGraph("images/bgextra.png");
	backImg[CLOUD1_B] = LoadGraph("images/cloud1.png");
	backImg[CLOUD2_B] = LoadGraph("images/cloud2.jpg");
	backImg[GROUND] = DerivationGraph(0, 156, 512, 84, b);
	backImg[MOUTAINS] = DerivationGraph(0, 0, 512, 84, b);
	backImg[MENU_B] = DerivationGraph(256, 160, 64, 64, a);
	//UIグラフィック
	UIImg[MIGI] = LoadGraph("images/migi.png");
	UIImg[HIDARI] = LoadGraph("images/hidari.png");
	UIImg[SHOTFLAME] = LoadGraph("images/waku01.png");
	UIImg[ROBO_ICON] = LoadGraph("images/RoboIcon.bmp");
	UIImg[RUNE_FLAME] = LoadGraph("images/runeFlame.png");
	UIImg[BLUE_FLAME] = LoadGraph("images/blueFlame.png");
	UIImg[DUST_BOX] = LoadGraph("images/dust.png");
	UIImg[NEWGAME_GFX] = LoadGraph("images/newgame.png");
	UIImg[LOADGAME_GFX] = LoadGraph("images/loadgame.png");
	UIImg[EXIT_GFX] = LoadGraph("images/exit.png");
	UIImg[DATA1_GFX] = LoadGraph("images/data1.png");
	UIImg[DATA2_GFX] = LoadGraph("images/data2.png");
	UIImg[DATA3_GFX] = LoadGraph("images/data3.png");
	UIImg[POWA] = LoadGraph("images/powa.png");
	UIImg[PLAYER_FLAME] = LoadGraph("images/waku00.png");
	DeleteGraph(a);

	//データとしてはプレイヤーが生成されてる。セーブのロードはしてない
	JetManager *jm = JetManager::getInstance();

	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);
	cursor->graphInit();
	GetWindowSize(&winWidth, &winHeight);
	GetWindowSize(&battleWidth, &battleHeight);
	battleHeight -= UI_ZONE;
	jm->init();

}
void GameManager::titleInit() {
	gameScene = title;
	scene_title = new titlescene();
}
void GameManager::titleEnd() {
	SAFE_DELETE(scene_title);
}
void GameManager::menuInit() {
	gameScene = menu;
	scene_menu = new menuscene();
}
void GameManager::menuEnd() {
	SAFE_DELETE(scene_menu);
}


//敵を生成
void GameManager::playInit(int stageNum) {
	JetManager *jm = JetManager::getInstance();
	gameScene = play;

	scene_play = new playscene();
	//敵機作成
	switch (stageNum)
	{
	case 0:
		jm->genStage0();
		break;
	case 1:
		jm->genStage1();
		break;
	case 2:
		jm->genStage2();
		break;
	case 3:
		jm->genStage3();
		break;
	default:
		break;
	}
	jm->player->playerInit();
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