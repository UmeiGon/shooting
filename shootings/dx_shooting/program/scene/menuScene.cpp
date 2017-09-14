#include "menuScene.h"
#include "../gameManager.h"
#include "../playerJet.h"
#include "../jetManager.h"
#include "../rune.h"
#include "../save.h"
//aとbのテキストと画像を交換
void menuscene::buttonchange(ClickBox* a, ClickBox* b) {
	ClickBox c = *a;
	a->text = b->text;
	a->text2 = b->text2;
	a->handle = b->handle;
	a->offsettextX = b->offsettextX;
	a->offsettextY = b->offsettextY;
	b->text = c.text;
	b->text2 = c.text2;
	b->handle = c.handle;
	b->offsettextX = c.offsettextX;
	b->offsettextY = c.offsettextY;
}

void menuscene::runedrop() {
	JetManager* jm = JetManager::getInstance();
	GameManager* gm = GameManager::getInstance();
	DrawRotaGraph(gm->winWidth / 2, wepy[0], 1.6f, 0, gm->UIImg[GameManager::ROBO_ICON], false);

	runeBixBox->update();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 88);
	//何もしない
	for (int i = 0; i < PlayerJet::SET_RUNE_SUU; i++) {
		setblackboxs[i]->update();
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	for (int i = 0; i < PlayerJet::MAX_RUNE_SUU; i++) {
		if (i != drag&&jm->player->runes[i] && runeBox[i]->update()) {
			drag = i;
		}
	}
	for (int i = 0; i < PlayerJet::SET_RUNE_SUU; i++) {
		if (i != setdrag&&jm->player->setRunes[i] && runeSetBox[i]->update()) {
			setdrag = i;
		}
	}
	dustBox->update();
	if (setdrag != -1) {
		if (GetMouseInput()&MOUSE_INPUT_LEFT) {
			DrawRotaGraph(gm->cursor->mouseX, gm->cursor->mouseY, 2.0f, 0, runeSetBox[setdrag]->handle, true);
		}
		else {
			//ゴミ箱
			if (dustBox->sawari()) {
				SAFE_DELETE(jm->player->setRunes[setdrag]);
				runeSetBox[setdrag]->handle = -2;
				runeSetBox[setdrag]->text = "";
				runeSetBox[setdrag]->text2 = "";
			}
			//離した瞬間
			for (int i = 0; i < PlayerJet::SET_RUNE_SUU; i++) {
				if (runeSetBox[i]->sawari()) {
					if (jm->player->setRunes[i]) {
						//上と上のルーンを交換
						Rune* ru = jm->player->setRunes[i];
						jm->player->setRunes[i] = jm->player->setRunes[setdrag];
						jm->player->setRunes[setdrag] = ru;
					}
					else {
						jm->player->setRunes[i] = jm->player->setRunes[setdrag];
						jm->player->setRunes[setdrag] = nullptr;
					}
					buttonchange(runeSetBox[i], runeSetBox[setdrag]);
				}
			}
			for (int k = 0; k < PlayerJet::MAX_RUNE_SUU; k++) {
				if (runeBox[k]->sawari()) {
					//上と下のルーンを入れ替え
					if (jm->player->runes[k]) {
						Rune* ru = jm->player->runes[k];
						jm->player->runes[k] = jm->player->setRunes[setdrag];
						jm->player->setRunes[setdrag] = ru;
					}
					else {
						jm->player->runes[k] = jm->player->setRunes[setdrag];
						jm->player->setRunes[setdrag] = nullptr;
					}
					buttonchange(runeBox[k], runeSetBox[setdrag]);
				}
			}
			setdrag = -1;

		}
	}
	else if (drag != -1) {
		if (GetMouseInput()&MOUSE_INPUT_LEFT) {
			DrawRotaGraph(gm->cursor->mouseX, gm->cursor->mouseY, 2.0f, 0, runeBox[drag]->handle, true);
		}
		else {
			//ゴミ箱
			if (dustBox->sawari()) {
				SAFE_DELETE(jm->player->runes[drag]);
				runeBox[drag]->handle = -2;
				runeBox[drag]->text = "";
				runeBox[drag]->text2 = "";
			}
			for (int i = 0; i < PlayerJet::SET_RUNE_SUU; i++) {
				if (runeSetBox[i]->sawari()) {
					if (jm->player->setRunes[i]) {
						//下と上ルーンを交換
						Rune* ru = jm->player->setRunes[i];
						jm->player->setRunes[i] = jm->player->runes[drag];
						jm->player->runes[drag] = ru;
					}
					else {
						jm->player->setRunes[i] = jm->player->runes[drag];
						jm->player->runes[drag] = nullptr;
					}
					buttonchange(runeSetBox[i], runeBox[drag]);
				}
			}
			for (int k = 0; k < PlayerJet::MAX_RUNE_SUU; k++) {
				if (runeBox[k]->sawari()) {
					//下のルーンを入れ替え
					if (jm->player->runes[k]) {
						Rune* ru = jm->player->runes[k];
						jm->player->runes[k] = jm->player->runes[drag];
						jm->player->runes[drag] = ru;
					}
					else {
						jm->player->runes[k] = jm->player->runes[drag];
						jm->player->runes[drag] = nullptr;
					}
					buttonchange(runeBox[k], runeBox[drag]);
				}
			}
			drag = -1;
		}
	}

}
bool menuscene::update() {
	GameManager* gm = GameManager::getInstance();
	JetManager* jm = JetManager::getInstance();
	//背景
	for (int x = 1; x < gm->winWidth; x += 64) {
		for (int y = 1; y < gm->winHeight; y += 64) {
			DrawGraph(x, y, gm->backImg[gm->MENU_B], false);
		}
	}


	//exit
	if (exitButton->update()) {
		DxLib_End();
	}


	//メニューシーンを切り替え
	if (gm->input->isKeyDownTrigger(KEY_INPUT_LEFT)) {
		sce--;
		if (sce < 0)sce = 3;
	}
	else if (gm->input->isKeyDownTrigger(KEY_INPUT_RIGHT)) {
		sce++;
		if (sce > 3)sce = 0;
	}
	if (gm->input->isKeyDownTrigger(KEY_INPUT_F4)) {
		jm->player->ironsuu += 100;
	}
	//シーンごとの処理
	switch (sce)
	{
	case menuscene::RUNE_HEYA:
		SetFontSize(60);
		DrawString(gm->winWidth / 2, 0, "ルーン", 0xffffff);
		SetFontSize(16);
	
		for (int atype = 0; atype < PlayerJet::MAX_ARM_SUU; atype++) {
			if (leftBox[atype]->update()) {
				//左を押したら実体のショットが出るまでマイナスを繰り返す。
				for (int k = jm->player->nowShot[atype] - 1; jm->player->nowShot[atype] != k; k--) {
					if (k < 0)k = PlayerJet::MAX_SHOT_TYPE - 1;

					if (jm->player->shotData[atype][k]) {
						jm->player->nowShot[atype] = (PlayerJet::pShotType)k;
						break;
					}
				}
			}
			if (rightBox[atype]->update()) {
				//右を押したら出るまでプラス		
				for (int k = jm->player->nowShot[atype] + 1; jm->player->nowShot[atype] != k; k++) {
					if (k > PlayerJet::MAX_SHOT_TYPE - 1)k = 0;
					if (jm->player->shotData[atype][k]) {
						jm->player->nowShot[atype] = (PlayerJet::pShotType)k;
						break;
					}
				}
			}
		}

		jm->shotIconDraw(wepx[0], wepy[0] - wepsize / 2, PlayerJet::MAIN, jm->player->nowShot[PlayerJet::MAIN], wepsize);
		jm->shotIconDraw(wepx[1], wepy[1] - wepsize / 2, PlayerJet::SUB, jm->player->nowShot[PlayerJet::SUB], wepsize);
		jm->shotIconDraw(wepx[2], wepy[2] - wepsize / 2, PlayerJet::ULT, jm->player->nowShot[PlayerJet::ULT], wepsize);
		runedrop();

		break;
	case menuscene::SAVE_HEYA:
		SetFontSize(60);
		DrawString(gm->winWidth / 2, 0, "セーブ", 0xffffff);
		SetFontSize(16);
		for (int i = 0; i < GameManager::SAIDAI_DATA; i++) {
			if (dataButton[i]->update()) {
				gm->Save(i);
				dataButton[i]->text = "CREATED";
				gm->saveflag[i] = true;
			}
		}
		break;
	case menuscene::SHOP_HEYA:
		SetFontSize(60);
		DrawString(gm->winWidth / 2, 0, "ショップ", 0xffffff);
		SetFontSize(40);
		DrawRotaGraph(50, 50, 4.0f, 0, jm->itemGfx[JetManager::IRON], true);
		DrawFormatString(100, 50, 0xffffff, "X%d", jm->player->ironsuu);
		for (int i = 0; i < PlayerJet::MAX_ARM_SUU; i++) {
			for (int k = 0; k < PlayerJet::MAX_SHOT_TYPE; k++) {
				if (weaponShop[i][k].update()) {
					shotd sho = *jm->player->shotDataSet((PlayerJet::armtype)i, (PlayerJet::pShotType)k);
					if (sho.hituIron <= jm->player->ironsuu) {
						jm->player->ironsuu -= sho.hituIron;
						jm->player->shotData[i][k] = jm->player->shotDataSet((PlayerJet::armtype)i, (PlayerJet::pShotType)k);
						weaponShop[i][k].text = "SOLD_OUT";
						weaponShop[i][k].hantei = false;
					}
				}
			}
		}
		SetFontSize(16);
		break;
	case MAP_HEYA:
		SetFontSize(60);
		DrawString(gm->winWidth / 2, 0, "マップ", 0xffffff);		
		SetFontSize(40);
		for (int i = 0; i < 4; i++) {
			if (stageButton[i].update()) {
				gm->nowStage = i;
				return true;
			}
		}
		SetFontSize(16);
		break;
	default:
		break;
	}
	return false;
}
menuscene::menuscene() {
	JetManager* jm = JetManager::getInstance();
	GameManager* gm = GameManager::getInstance();
	dustBox = new ClickBox(40, gm->winHeight - 100, 80, 80, "", 0, 0, gm->UIImg[GameManager::DUST_BOX], false);
	drag = -1;
	setdrag = -1;
	sce = RUNE_HEYA;
	int size = 50;
	wepsize = 160;
	int wep = wepsize;
	int center = gm->winWidth / 2;
	int offset = (wep + size * 2) / 2;
	int aida = 350;
	int sx[3]{ center - aida - offset,center + aida - offset,center - offset };
	int sy[3]{ 450,450,150 };
	for (int i = 0; i < 3; i++) {
		wepx[i] = sx[i] + size;
		wepy[i] = sy[i];
	}
	for (int i = 0; i < PlayerJet::MAX_ARM_SUU; i++) {
		for (int k = 0; k < PlayerJet::MAX_SHOT_TYPE; k++) {
			shotd* so = jm->player->shotDataSet((PlayerJet::armtype)i, (PlayerJet::pShotType)k);
			if (so) {
				string strd = to_string(so->hituIron);
				bool buy = true;
				if (jm->player->shotData[i][k]) {
					strd = "SOLD_OUT";
					buy = false;
				}
				weaponShop[i][k] = ClickBox(250 + k * 200, 250 + i * 200, 150, 150, strd, 0, 0, so->gfx, buy);
			}
			SAFE_DELETE(so);
		}
	}
	for (int i = 0; i < 4; i++) {
		stageButton[i]=ClickBox(120+300*i,500,200,150,"STAGE_"+(to_string(i)),0,0);
		stageButton[i].color = GetColor(0,0,0);
	}
	memset(runeBox, 0, sizeof(runeBox));
	memset(runeSetBox, 0, sizeof(runeSetBox));
	for (int i = 0; i < GameManager::SAIDAI_DATA; i++) {
		int gx, gy;
		GetGraphSize(gm->UIImg[GameManager::DATA1_GFX + i], &gx, &gy);
		FILE* fp;
		string datastr;
		switch (i)
		{
		case 0:
			fp = fopen("savedata1.dat", "rb");
			break;
		case 1:
			fp = fopen("savedata2.dat", "rb");
			break;
		case 2:
			fp = fopen("savedata3.dat", "rb");
			break;
		default:
			break;
		}
		if (fp&&fgetc(fp) == -1) {
			datastr = "NO_DATA";
			gm->saveflag[i] = false;
		}
		else {
			datastr = "CREATED";
			gm->saveflag[i] = true;
		}
		dataButton[i] = new ClickBox((gm->winWidth / 2) - gx / 2, gm->winHeight / 6 * (3 + i), gx, gy, datastr, -120, 0, gm->UIImg[GameManager::DATA1_GFX + i],true, gm->UIImg[GameManager::POWA]);
		fclose(fp);
	}
	int rx, ry;
	GetGraphSize(gm->UIImg[GameManager::ROBO_ICON], &rx, &ry);
	rx *= 1.6f;
	ry *= 1.6f;
	int off = 220;
	int hei = 310;
	int wid = gm->winWidth - 220 * 2;
	t2k::vec3 robopos(gm->winWidth / 2 - rx / 2, wepy[0] - ry / 2, 0);
	runeBixBox = new ClickBox(220, gm->winHeight - 330, wid, hei, "", 0, 0, gm->UIImg[GameManager::SHOTFLAME], false);
	setblackboxs[0] = new ClickBox(robopos.x + 57 * 1.6f, robopos.y + 17 * 1.6f, 60, 60, "", 65, 0, gm->UIImg[GameManager::RUNE_FLAME]);
	setblackboxs[1] = new ClickBox(robopos.x + 135 * 1.6f, robopos.y + 47 * 1.6f, 60, 60, "", 65, 0, gm->UIImg[GameManager::RUNE_FLAME]);
	setblackboxs[2] = new ClickBox(robopos.x + 185 * 1.6f, robopos.y + 18 * 1.6f, 60, 60, "", 65, 0, gm->UIImg[GameManager::RUNE_FLAME]);
	setblackboxs[3] = new ClickBox(robopos.x + 183 * 1.6f, robopos.y + 163 * 1.6f, 60, 60, "", 65, 0, gm->UIImg[GameManager::RUNE_FLAME]);
	setblackboxs[4] = new ClickBox(robopos.x + 65 * 1.6f, robopos.y + 149 * 1.6f, 60, 60, "", 65, 0, gm->UIImg[GameManager::RUNE_FLAME]);
	for (int i = 0; i < PlayerJet::SET_RUNE_SUU; i++) {
		runeSetBox[i] = new ClickBox();
		*runeSetBox[i] = *setblackboxs[i];
		if (jm->player->setRunes[i]) {
			runeSetBox[i]->text = gm->runeStr(jm->player->setRunes[i]->type);
			runeSetBox[i]->text2 = "Lv:" + to_string(jm->player->setRunes[i]->kouka);
			runeSetBox[i]->handle = jm->itemGfx[2 + jm->player->setRunes[i]->type];
		}
		else {
			runeSetBox[i]->handle = -2;
		}
	}

	exitButton = new ClickBox(gm->winWidth - 100, 20, 80, 30, "", 0, 0, gm->UIImg[GameManager::EXIT_GFX]);

	leftBox[PlayerJet::MAIN] = new ClickBox(sx[0], sy[0], size, size, "", 0, 0, gm->UIImg[GameManager::HIDARI]);
	leftBox[PlayerJet::SUB] = new ClickBox(sx[1], sy[1], size, size, "", 0, 0, gm->UIImg[GameManager::HIDARI]);
	leftBox[PlayerJet::ULT] = new ClickBox(sx[2], sy[2], size, size, "", 0, 0, gm->UIImg[GameManager::HIDARI]);
	rightBox[PlayerJet::MAIN] = new ClickBox(sx[0] + wep + size, sy[0], size, size, "", 0, 0, gm->UIImg[GameManager::MIGI]);
	rightBox[PlayerJet::SUB] = new ClickBox(sx[1] + wep + size, sy[1], size, size, "", 0, 0, gm->UIImg[GameManager::MIGI]);
	rightBox[PlayerJet::ULT] = new ClickBox(sx[2] + wep + size, sy[2], size, size, "", 0, 0, gm->UIImg[GameManager::MIGI]);
	int xoff = off + 10;
	int yoff = gm->winHeight - 320;
	hei = 300;
	wid = gm->winWidth - xoff * 2;
	int count = 0;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 10; x++) {
			if (jm->player->runes[y * 10 + x]) {
				runeBox[y * 10 + x] = new ClickBox(xoff + wid / 10 * x, yoff + hei / 3 * y, 60, 60, gm->runeStr(jm->player->runes[y * 10 + x]->type), 65, 0, jm->itemGfx[(jm->player->runes[y * 10 + x]->type + 2)]);
				runeBox[y * 10 + x]->text2 = "Lv:" + to_string(jm->player->runes[y * 10 + x]->kouka);
			}
			else {
				runeBox[y * 10 + x] = new ClickBox(xoff + wid / 10 * x, yoff + hei / 3 * y, 60, 60, "", 65, 0, -2);
			}
		}
	}
}
menuscene::~menuscene() {
	SAFE_DELETE(runeBixBox);
	for (int i = 0; i < PlayerJet::SET_RUNE_SUU; i++) { SAFE_DELETE(setblackboxs[i]); }
	for (int i = 0; i < PlayerJet::MAX_ARM_SUU; i++) { SAFE_DELETE(leftBox[i]); }
	for (int i = 0; i < PlayerJet::MAX_ARM_SUU; i++) { SAFE_DELETE(rightBox[i]); }
	for (int i = 0; i < PlayerJet::MAX_RUNE_SUU; i++) { SAFE_DELETE(runeBox[i]); }
	for (int i = 0; i < PlayerJet::SET_RUNE_SUU; i++) { SAFE_DELETE(runeSetBox[i]); }
	for (int i = 0; i < GameManager::SAIDAI_DATA; i++) { SAFE_DELETE(dataButton[i]); }
	SAFE_DELETE(exitButton);
	SAFE_DELETE(dustBox);
}