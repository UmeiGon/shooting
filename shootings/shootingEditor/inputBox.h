#pragma once
#include <string>
using namespace std;
class ClickInputFlame {
public:
	int x = 0;
	int y = 0;
	int Fwidth = 120;
	int Fheight = 60;
	int handle;
	int offsettextX;
	int offsettextY;
	string text;
	string text2;
	ClickInputFlame(int ax, int ay, int w, int h, string ptext, int textx = 0, int texty = 0, int gfx = -1);
	//テキストと四角を描いて押されたらtrueを返す関数
	bool update();
};