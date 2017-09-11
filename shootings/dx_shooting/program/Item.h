#pragma once
#include "TamaJet.h"
class Item:public Tama{
public:
	enum type { IRON, ENERGY, RUNE };
	int tag;
	Item(float X, float Y, float Size, int Tag,float Spd);
	int runeType;
	float runekouka;
	void ItemUpdate();
};