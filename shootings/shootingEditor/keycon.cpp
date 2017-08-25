#include <DxLib.h>
#include "keycon.h"

keymouseInput* keymouseInput::getInstance() {
	keymouseInput* instance = nullptr;
	if (instance == nullptr) {
		instance = new keymouseInput;
	}
	return instance;
}
int keymouseInput::keyDownCheck() {
	char tmpkey[256];
	GetHitKeyStateAll(tmpkey);
	for (int i = 0; i < 256; i++) {
		if (tmpkey[i] != 0) {
			iKey[i]++;
		}
		else {
			iKey[i] = 0;
		}
		if (iKey[i] == 1) {
			KeyDown[i] = true;
		}
		else {
			KeyDown[i] = false;
		}
	}
	return 0;
}
int keymouseInput::mouseDownCheck() {
	int gme = GetMouseInput();
	for (int i = 0; i < 2; i++) {
		if ((gme&(MOUSE_INPUT_LEFT+i)) != 0) {
			iMouse[i]++;
		}
		else {
			iMouse[i] = 0;
		}
		if (iMouse[i] == 1) {
			MouseDown[i] = true;
		}
		else {
			MouseDown[i] = false;
		}
	}
	return 0;
}
bool keymouseInput::isMouseDownTrigger(int num) {
	if (MouseDown[num-1]==1) {
		return true;
	}
	return false;
}
bool keymouseInput::isKeyDownTrigger(int num) {
	if (KeyDown[num]==1) {
		return true;
	}
	return false;
}



