#pragma once
#include <string>
using namespace std;
class ClickBox {
public:
	int x = 0;
	int y = 0;
	int Fwidth = 120;
	int Fheight = 60;
	int handle;
	int handle2;
	int offsettextX;
	int offsettextY;
	int color;
	bool hantei;
	string text;
	string text2;
	ClickBox(int ax, int ay, int w, int h, string ptext, int textx = 0, int texty = 0, int gfx = -1,bool clickhantei=true,int gfx2=-1);
	ClickBox();
	//テキストと四角を描いて押されたらtrueを返す関数
	bool update();
	bool sawari();
};