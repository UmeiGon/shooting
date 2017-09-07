#pragma once
class ClickBox;
class menuscene {
public:
	menuscene();
	ClickBox *leftBox[3];
	ClickBox *rightBox[3];

	bool update();
};