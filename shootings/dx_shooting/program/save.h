#pragma once
#include "PlayerJet.h"
#include "rune.h"
#include <string.h>
class saveData {
public:
	saveData();
	Rune runes[30];
	Rune setRunes[5];
	int ironkazu;
	bool weaponflag[PlayerJet::MAX_ARM_SUU][PlayerJet::MAX_SHOT_TYPE];
	void save(int saveN);
	void load(int saveN);
};