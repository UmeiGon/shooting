#include "Dxlib.h"
#include "titleScene.h"
#include "../inputBox.h"
//#include "../gameManager.h"
//#include "../capsule.h"
//#include "../keycon.h"
titlescene::titlescene() {
	startButton = new ClickBox(400, 600, 150, 80, "Start");
}
//Capsule* cap1=nullptr;
//Capsule* cap2=nullptr;
//t2k::vec3 *point;
//int r = 60;
bool titlescene::update() {
	


	if (startButton->update()) {
		return true;
	}
	return false;
}