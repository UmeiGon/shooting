#include "debug.h"
#include "Dxlib.h"
#include "gameManager.h"
#include "cursor.h"
Debug::Debug() {
	preTime = 0;
	dTime = 0.06f;
	halfTimer = 0;
	myfps = 0;
	showDebug = false;
}
Debug* Debug::getInstance() {
	static Debug* instance = nullptr;
	if (nullptr == instance) {
		instance = new Debug();
	}
	return instance;
}
void Debug::update() {
	GameManager* gm = GameManager::getInstance();
	int nowtime = GetNowCount();
	int satime;
	if (preTime != 0) {
		satime = nowtime - preTime;
		dTime = (double)(satime) / 1000.0;
		halfTimer += satime;
	}
	preTime = nowtime;
	if (halfTimer > 500) {//1/2•b–ˆ‚Ìˆ—
		myfps = 1000 / (satime);
		halfTimer = 0;
	}
	if (showDebug) {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "X=%d Y=%d,\nfps=%d,\ntime=%d,\ndtime=%f\nobj=%d\n", gm->cursor->mouseX, gm->cursor->mouseY, myfps, nowtime, dTime, objSuu);
	}
}