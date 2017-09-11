#pragma once
class ClickBox;
class menuscene {
public:
	menuscene();
	ClickBox *leftBox[3];
	ClickBox *rightBox[3];
	int wepx[3];
	int wepy[3];
	int wepsize = 160;
	bool update();
};