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
	switch (tag)
	{
	case 0:
		handle = jm->itemGfx[JetManager::IRON];
		break;
	case 1:
		handle = jm->itemGfx[JetManager::ENERGY];
		break;
	case 2:
		runeType = GetRand(4);
		switch (runeType)
		{
		case JetManager::HOUNETU_RUNE:
			handle = jm->itemGfx[JetManager::HOUNETU];
			runekouka = (float)(GetRand(8) + 1) * 0.015f + 0.4f;
			break;
		case JetManager::CD_RUNE:
			handle = jm->itemGfx[JetManager::CD];
			runekouka = (float)(GetRand(8) + 1) * 0.01 + 0.05f;
			break;
		case JetManager::SPD_RUNE:
			handle = jm->itemGfx[JetManager::SPD];
			runekouka = (float)(GetRand(8) + 1) * 0.012f + 0.2f;
			break;
		case JetManager::ATK_RUNE:
			handle = jm->itemGfx[JetManager::ATK];
			runekouka = (float)(GetRand(8) + 1) * 0.02f + 0.2f;
			break;
		default:
			break;
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
	circle.pos.x -= gm->debug->dTime*speed*120.0f;
	if (circle.pos.y < gm->battleHeight) {
		circle.pos.y += gm->debug->dTime*speed*35.0f;
	}
	draw();
}