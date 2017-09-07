#pragma once
class ClickBox;
class resultscene {

public:
	ClickBox *retryButton;
	enum go{NONE,GO_MENU,GO_PLAY};
	resultscene(int kekka = -1);
	go update();
};