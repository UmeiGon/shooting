#include "inputBox.h"
#include "Dxlib.h"
#include "cursor.h"
#include "keycon.h"
#include "gameManager.h"
ClickBox::ClickBox(int ax, int ay, int w, int h, string ptext, int textx, int texty, int gfx) {
	x = ax;
	y = ay;
	Fwidth = w;
	Fheight = h;
	text = ptext;
	handle = gfx;
	offsettextX = textx;
	offsettextY = texty;
}
bool ClickBox::update() {
	GameManager* gm = GameManager::getInstance();
	if (handle >= 0) {
		int w, h;
		GetGraphSize(handle, &w, &h);
		DrawRotaGraph(x, y,(int)Fwidth/(int)w,0, handle,true);
	}
	else {
		DrawBox(x, y, x + Fwidth, y + Fheight, 0xffffff, TRUE);
	}
	DrawFormatString(x + offsettextX, y + offsettextY + 5, GetColor(20, 25, 25), text.c_str());
	DrawFormatString(x + offsettextX, y + offsettextY + GetFontSize()*1.5f, GetColor(20, 25, 25), text2.c_str());
	if (x <= gm->cursor->mouseX&&gm->cursor->mouseX <= x + Fwidth && y <= gm->cursor->mouseY&&gm->cursor->mouseY <= y + Fheight) {
		gm->cursor->cNum = Cursor::click;
		if (gm->input->isMouseDownTrigger(MOUSE_INPUT_LEFT)) {
			return true;
		}
	};
	return false;
}