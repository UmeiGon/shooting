#include "Dxlib.h"
#include "titleScene.h"
#include "../inputBox.h"
#include "../gameManager.h"
titlescene::titlescene() {
	startButton = new ClickBox(400, 600, 150, 80, "Start");
}
//Capsule* cap1=nullptr;
//Capsule* cap2=nullptr;
//t2k::vec3 *point;
//int r = 60;

bool init = false;
bool titlescene::update() {
	GameManager* gm =GameManager::getInstance();
	DrawGraph(0,0,gm->backImg[gm->MENU_B],false);
	if (startButton->update()) {
			return true;
		}
	return false;
}