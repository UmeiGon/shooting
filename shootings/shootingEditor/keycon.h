#include <DXLib.h>
#pragma once

class keymouseInput {

private:
	int iKey[256];
	int iMouse[2];
	bool KeyDown[256];
	bool MouseDown[2];
public:
	static keymouseInput* getInstance();
	int keyDownCheck();
	int mouseDownCheck();
	bool isMouseDownTrigger(int);
	bool isKeyDownTrigger(int);
};