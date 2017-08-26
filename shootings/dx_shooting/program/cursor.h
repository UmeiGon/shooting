#pragma once
class Cursor {
public:
	static Cursor* getInstance();
	int mouseX;
	int mouseY;
	int Image[3];
	enum cursorNum { def, click, input };
	cursorNum cNum;
	void cursorImageInit();
	void cursorDraw();
	void graphInit();
};