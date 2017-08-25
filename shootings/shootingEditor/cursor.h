#pragma once
class Cursor {
public:
	static Cursor* getInstance();
	int mouseX;
	int mouseY;
	int cursorImage[3];
	enum cursorNum { def, click, input };
	cursorNum cNum;
	void cursorImageInit();
	void cursorDraw();
};