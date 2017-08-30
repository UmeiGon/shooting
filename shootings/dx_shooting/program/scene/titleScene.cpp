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
	//if (!cap1) {
	//	cap1 = new Capsule(t2k::vec3(90,40,0),t2k::vec3(69,222,0),40);
	//	point = new t2k::vec3(600, 400, 0);
	//}
	//Capsule::testCap(cap1, new Capsule, point, r);
	if (startButton->update()) {
		return true;
	}
	return false;
}