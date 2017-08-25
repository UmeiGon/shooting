#include "cursor.h"
#include "Dxlib.h"
Cursor* Cursor::getInstance() {
	static Cursor* instance = nullptr;
	if (nullptr == instance) {
		instance = new Cursor();
	}
	return instance;
}
void Cursor::cursorImageInit() {
	cursorImage[def] = LoadGraph("../dx_shooting/images/defaultCursor.png");
	cursorImage[click] = LoadGraph("../dx_shooting/images/clickCursor.png");
	cursorImage[input] = LoadGraph("../dx_shooting/images/inputCursor.png");
}
void Cursor::cursorDraw() {
	DrawRotaGraph(mouseX, mouseY, 1.5, 0, cursorImage[cNum], true);
}