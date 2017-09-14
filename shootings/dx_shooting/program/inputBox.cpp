#include "inputBox.h"
#include "Dxlib.h"
#include "cursor.h"
#include "keycon.h"
#include "gameManager.h"
ClickBox::ClickBox(int ax, int ay, int w, int h, string ptext, int textx, int texty, int gfx, bool clickhantei, int gfx2) {
	x = ax;
	y = ay;
	Fwidth = w;
	Fheight = h;
	text = ptext;
	handle = gfx;
	handle2 = gfx2;
	offsettextX = textx;
	offsettextY = texty;
	hantei = clickhantei;
	color = 0xffffff;
}
ClickBox::ClickBox() {
	handle2 = -1;
};
bool ClickBox::update() {
	GameManager* gm = GameManager::getInstance();
	if (handle != -1) {
		int w, h;
		GetGraphSize(handle, &w, &h);
		DrawModiGraph(x, y, x + Fwidth, y, x + Fwidth, y + Fheight, x, y + Fheight, handle, true);
	}
	else DrawBox(x, y, x + Fwidth, y + Fheight, 0xffffff, TRUE);
	if (text != "") {
		DrawFormatString(x + offsettextX, y + offsettextY + 5, color, text.c_str());
	}
	if (handle2 != -1) {
		if (sawari()) {
			DrawModiGraph(x, y, x + Fwidth, y, x + Fwidth, y + Fheight, x, y + Fheight, handle2, true);
		}
	}

	if (text2 != "") {
		DrawFormatString(x + offsettextX, y + offsettextY + GetFontSize()*1.5f, color, text2.c_str());
	}

	if (hantei&&x <= gm->cursor->mouseX&&gm->cursor->mouseX <= x + Fwidth && y <= gm->cursor->mouseY&&gm->cursor->mouseY <= y + Fheight) {
		gm->cursor->cNum = Cursor::click;
		if (gm->input->isMouseDownTrigger(MOUSE_INPUT_LEFT)) {
			return true;
		}
	};
	return false;
}
bool ClickBox::sawari() {
	GameManager* gm = GameManager::getInstance();
	if (x <= gm->cursor->mouseX&&gm->cursor->mouseX <= x + Fwidth && y <= gm->cursor->mouseY&&gm->cursor->mouseY <= y + Fheight) {
		return true;
	}
	return false;
}