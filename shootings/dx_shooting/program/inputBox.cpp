#include "inputBox.h"
#include "Dxlib.h"
#include "cursor.h"
#include "keycon.h"
ClickInputFlame::ClickInputFlame(int ax, int ay, int w, int h, string ptext, int textx , int texty , int gfx) {
	x = ax;
	y = ay;
	Fwidth = w;
	Fheight = h;
	text = ptext;
	handle = gfx;
	offsettextX = textx;
	offsettextY = texty;
}
bool ClickInputFlame::update() {
	Cursor* cursor = Cursor::getInstance();
	keymouseInput* input = keymouseInput::getInstance();
	if (handle >= 0) {
		DrawGraph(x, y, handle, true);
	}
	else {
		DrawBox(x, y, x + Fwidth, y + Fheight, 0xffffff, TRUE);
	}
	DrawFormatString(x + offsettextX, y + offsettextY + 5, GetColor(20, 25, 25), text.c_str());
	DrawFormatString(x + offsettextX, y + offsettextY + 21, GetColor(20, 25, 25), text2.c_str());
	if (x <= cursor->mouseX&&cursor->mouseX <= x + Fwidth && y <= cursor->mouseY&&cursor->mouseY <= y + Fheight) {
		cursor->cNum = Cursor::click;
		if (input->isMouseDownTrigger(MOUSE_INPUT_LEFT)) {
			return true;
		}
	};
	return false;
}