#include "Item.h"
#include "Dxlib.h"
#include "Jetmanager.h"
#include "GameManager.h"
Item::Item(float X, float Y, float Size, int Tag, float Spd) :Tama() {
	JetManager* jm = JetManager::getInstance();
	circle.pos.x = X;
	circle.pos.y = Y;
	circle.r = Size / 2;
	size = Size;
	speed = Spd;
	tag = Tag;
	int r;
	switch (tag)
	{
	case 0:
		handle = jm->itemGfx[JetManager::IRON];
		break;
	case 1:
		handle = jm->itemGfx[JetManager::ENERGY];
		break;
	case 2:
		runeType = GetRand(3);
		switch (runeType)
		{
		case JetManager::HOUNETU_RUNE:
			handle = jm->itemGfx[JetManager::HOUNETU_GFX];
			break;
		case JetManager::CD_RUNE:
			handle = jm->itemGfx[JetManager::CD_GFX];
			break;
		case JetManager::SPD_RUNE:
			handle = jm->itemGfx[JetManager::SPD_GFX];
			break;
		case JetManager::ATK_RUNE:
			handle = jm->itemGfx[JetManager::ATK_GFX];
			break;
		default:
			break;
		}
		r = GetRand(100);
		runekouka = 1;
		if (r % 2 == 0) {
			runekouka = 1;
		}
		else if (r % 3 == 0) {
			runekouka = 2;
		}
		else if (r % 5 == 0) {
			runekouka = 3;
		}
		else if (r % 7 == 0) {
			runekouka = 4;
		}
		else if (r % 11 == 0) {
			runekouka = 5;
		}
		break;
	default:
		break;
	}
	liveTimer = 0;
	capsule = nullptr;

	int gx;
	int gy;
	graphAngle = 0;
	GetGraphSize(handle, &gx, &gy);
	graphSize = size / gx;
}
//地面に着くまで下がり、常に右に行く。デリート処理はしてない。
void Item::ItemUpdate() {

	if (circle.pos.y < gm->battleHeight) {
		circle.pos.x -= gm->debug->dTime*speed*95.0f;
		float rakkaspd = 1;
		if (tag == JetManager::IRON) {
			rakkaspd*1.5f;
		}
		circle.pos.y += gm->debug->dTime*speed*85.0f*rakkaspd;
	}
	else {
		circle.pos.x -= gm->debug->dTime*speed*120.0f;
	}
	draw();
}
int Item::itemhaisyutu() {
	int a = GetRand(100);
	if (a<90) {
		return 0;
	}
	else if(a<95){
		return 1;
	}
	else{
		return 2;
	}
}