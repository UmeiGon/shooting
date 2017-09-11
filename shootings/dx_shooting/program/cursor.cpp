#include "cursor.h"
#include "Dxlib.h"
Cursor* Cursor::getInstance() {
	static Cursor* instance = nullptr;
	if (nullptr == instance) {
		instance = new Cursor();
	}
	return instance;
}
void Cursor::graphInit() {
	Image[def] = LoadGraph("../dx_shooting/images/defaultCursor.png");
	Image[click] = LoadGraph("../dx_shooting/images/clickCursor.png");
	Image[input] = LoadGraph("../dx_shooting/images/inputCursor.png");
}
void Cursor::cursorImageInit() {

}
void Cursor::cursorDraw() {
	DrawGraph(mouseX, mouseY, Image[cNum], true);
}