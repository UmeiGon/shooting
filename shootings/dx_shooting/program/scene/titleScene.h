#pragma once
#include "../gameManager.h"
class ClickBox;
class titlescene {
public:
	enum scenes{SAISYO,LOADSENTAKU};
	enum teius{};
	scenes sce;
	ClickBox *newgameButton;
	ClickBox *loadgameButton;
	ClickBox *exitButton;
	ClickBox *deleteButton[GameManager::SAIDAI_DATA];
	ClickBox *dataButton[GameManager::SAIDAI_DATA];
	titlescene();
	~titlescene();
	bool update();
};