#pragma once
#include "../playerJet.h"
#include "../gameManager.h"
#include "../inputBox.h"

class menuscene {
public:
	menuscene();
	~menuscene();
	ClickBox *runeBixBox;
	ClickBox *setblackboxs[PlayerJet::SET_RUNE_SUU];
	ClickBox *leftBox[PlayerJet::MAX_ARM_SUU];
	ClickBox *rightBox[PlayerJet::MAX_ARM_SUU];
	ClickBox *runeBox[PlayerJet::MAX_RUNE_SUU];
	ClickBox *runeSetBox[PlayerJet::SET_RUNE_SUU];
	ClickBox *dustBox;
	ClickBox *dataButton[GameManager::SAIDAI_DATA];
	ClickBox *exitButton;
	ClickBox weaponShop[PlayerJet::MAX_ARM_SUU][PlayerJet::MAX_SHOT_TYPE];
	ClickBox stageButton[4];
	const int SCENE_SUU=4;
	enum scenes {RUNE_HEYA,SAVE_HEYA,SHOP_HEYA,MAP_HEYA};
	int sce;
	int wepx[PlayerJet::MAX_ARM_SUU];
	int wepy[PlayerJet::MAX_ARM_SUU];
	int wepsize = 160;
	int drag;
	int setdrag;
	void runedrop();
	void buttonchange(ClickBox* a, ClickBox* b);
	bool update();
};