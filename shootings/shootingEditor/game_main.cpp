#include <DxLib.h>
#include <math.h>
#include "keycon.h"
#include <string>
#include "vec3.h"
#include "inputBox.h"
#include "cursor.h"
using namespace std;
#define SAFE_DELETE(p){\
	if(p){\
		delete(p);\
		(p)=nullptr;\
	}\
}

#define SAFE_DELETE_ARRAY(p){\
	if(p){\
		delete[](p);\
		(p)=nullptr;\
	}\
}
const int DEFAULT_SIZE = 16;
const int E_DATA_SIZE = 8;
int winWidth;
int winHeight;
int img[10];
float imasec;
typedef struct {
	int x = 0;
	int y = 0;
	int movetype = 0;
	int shottype = 0;
	int syutugenSec = 0;
	int deathSec = 0;
	int size = 1;
	int img = 0;
}ENEMY_DATA;
class mapData {

public:
	mapData() {
		memset(eDatas, 0, sizeof(eDatas));
	}
	int sec = 0;
	ENEMY_DATA *eDatas[100];
	void outputData() {
		for (int i = 0; i < 100; i++) {
			if (eDatas[i] && eDatas[i]->deathSec + imasec >= eDatas[i]->syutugenSec&&eDatas[i]->syutugenSec <= imasec) {
				float spinTimer = (eDatas[i]->syutugenSec - imasec) * 5;
				int x = 0;
				int y = 0;
				switch (eDatas[i]->movetype)
				{
				case 0:
					x = eDatas[i]->x + eDatas[i]->size + (eDatas[i]->size / 2)*cos(spinTimer);
					y = eDatas[i]->y + eDatas[i]->size + (eDatas[i]->size / 2)*sin(spinTimer);
					break;
				case 1:
					x = eDatas[i]->x - (eDatas[i]->syutugenSec - imasec) * 5;
					y = eDatas[i]->y + (eDatas[i]->syutugenSec - imasec) * 5;
					break;
				default:
					break;
				}
				int imgx;
				int imgy;
				GetGraphSize(img[eDatas[i]->img], &imgx, &imgy);
				DrawRotaGraph(x, y, (float)eDatas[i]->size / (float)(imgx), 0, img[eDatas[i]->img], true);
			}
		}
	}
};
mapData *mapdatas[30];
ENEMY_DATA *hyozi[100];
bool nullFileCheck(FILE* file) {
	if (file == NULL) {
		return false;
	}
	else {
		return true;
	}
}

//void FileTest() {
//	FILE* fp;
//	if (nullFileCheck(fp = fopen("test.dat", "wb"))) {
//		for (int i = 0; i < 10; i++) {
//			datas[i] = new ENEMY_DATA;
//			datas[i]->x = (i + 1) * 5;
//			datas[i]->y = (i + 1) * 5;
//			datas[i]->movetype = (i + 1) * 5;
//			datas[i]->deathSec = (i + 1) * 5.32f;
//			fwrite((ENEMY_DATA*)datas[i], sizeof(ENEMY_DATA), 1, fp);
//			SAFE_DELETE(datas[i]);
//		}
//		fclose(fp);
//	}
//
//}
//void FileTest2() {
//	FILE* fp;
//	if (nullFileCheck(fp = fopen("test.dat", "rb"))) {
//		for (int i = 0; i < 10; i++) {
//			hyozi[i] = new ENEMY_DATA;
//			fread((ENEMY_DATA*)hyozi[i], sizeof(ENEMY_DATA), 1, fp);
//		}
//		fclose(fp);
//	}
//
//}
float tumami() {
	static int x = 50;
	static float ratio;
	Cursor* cursor=Cursor::getInstance();
	if (50 <= cursor->mouseX&&cursor->mouseX <= winWidth - 50 && 0 <= cursor->mouseY&&cursor->mouseY <= 30 && (GetMouseInput() & MOUSE_INPUT_LEFT)) {
		x = cursor->mouseX;
		ratio = (float)(x - 50) / (float)(winWidth - 100);
	};
	DrawBox(50, 0, winWidth - 50, 30, 0xffffff, false);
	DrawBox(50, 0, x, 30, 0xfffffff, false);
	return ratio;
}
class textBox {
public:
	int x;
	int y;
	int Fwidth = 50;
	int Fheight = 25;
	string text;
	textBox(int ax, int ay, int w = 50, int h = 25, string t = "") {
		x = ax;
		y = ay;
		Fwidth = w;
		Fheight = h;
		text = t;
	}
	void update() {
		DrawBox(x, y, x + Fwidth, y + Fheight, 0xffffff, true);
		DrawFormatString(x + 10, y + 10, 0xffffff, "%s", text.c_str());
	}
};
class NumberInputFlame {
public:
	int* num = nullptr;
	int x = 0;
	int y = 0;
	int Fwidth;
	int Fheight;
	int maxNum;
	int textsize = 16;
	string setumeiText = "";
	NumberInputFlame(int ax, int ay, string sText = "", int w = 160, int h = 80, int maxnum = 9999) {
		num = new int;
		*num = 0;
		x = ax;
		y = ay;
		Fwidth = w;
		Fheight = h;
		textsize = 16 * ((float)h / (float)80);
		setumeiText = sText;
		maxNum = maxnum;
	}
	//貰った数値を連結して足していく
	void plusNum(int plusN) {

		if (plusN == -2) {
			return;
		}
		else if (plusN == -1) {
			if(*num>0)*num /= 10;
		}
		else {
			*num *= 10;
			*num += plusN;
		}
		if (*num > maxNum) {
			*num = maxNum;
		}
		if (*num < 0) {
			*num = 0;
		}

	}
	bool update(bool colActive = false) {
		Cursor* cursor = Cursor::getInstance();
		keymouseInput* input = keymouseInput::getInstance();
		int col;
		if (colActive) {
			col = GetColor(222, 222, 222);
		}
		else {
			col = GetColor(80, 80, 80);
		}
		DrawBox(x, y, x + Fwidth, y + Fheight, col, TRUE);
		int fontsize = GetFontSize();
		int setusize = 2 * Fwidth / (setumeiText.length());
		if (setusize > Fheight) {
			setusize = Fheight;
		}
		SetFontSize(setusize);
		DrawFormatString(x, y, GetColor(10, 10, 10), "%s", setumeiText.c_str());
		SetFontSize(textsize);
		DrawFormatString(x + 10, y + 5, GetColor(255, 255, 255), "%d", *num);
		SetFontSize(fontsize);
		if (x <= cursor->mouseX&&cursor->mouseX <= x + Fwidth && y <= cursor->mouseY&&cursor->mouseY <= y + Fheight) {
			cursor->cNum = Cursor::input;
			if (input->isMouseDownTrigger(MOUSE_INPUT_LEFT)) {
				return true;
			}
		};
		return false;
	}
};
NumberInputFlame *numBox[E_DATA_SIZE];
NumberInputFlame *SecBox;
NumberInputFlame *eNumBox;
class DataButton {
public:
	DataButton(int px, int py) {
		x = px;
		y = py;
		StartButton = new ClickInputFlame(x, y, 150, 80, "SAVE_DATA", 40, 40);
		CreateButton = new ClickInputFlame(x + 5, y, 32, 32, "", 0, 0, img[0]);
		DeleteButton = new ClickInputFlame(x + 5, y + 40, 32, 32, "", 0, 0, img[1]);
	}
	~DataButton() {
		SAFE_DELETE(StartButton);
		SAFE_DELETE(CreateButton);
		SAFE_DELETE(DeleteButton);
	}
	int x;
	int y;
	ClickInputFlame *StartButton;
	ClickInputFlame *CreateButton;
	ClickInputFlame *DeleteButton;
};
ClickInputFlame *eCreButton;
ClickInputFlame *eDelButton;
DataButton *dButtons[30];
//数字キーを押すとその値が返る関数
int suutiNyuryoku() {
	keymouseInput* input = keymouseInput::getInstance();
	for (int i = 0; i < 10; i++) {
		if (input->isKeyDownTrigger(KEY_INPUT_1+i-1)) {
			return (i + 1) % 10;
		}
	}
	if (input->isKeyDownTrigger(KEY_INPUT_BACK)) {
		return -1;
	}
	return -2;
}
void dataChoiceInit() {
	/*CreateButton = new ClickInputFlame(300, 500, "新規作成");
	DeleteButton = new ClickInputFlame(500, 500, "データ削除");*/
	for (int a = 0; a < 5; a++) {
		for (int b = 0; b < 6; b++) {
			int n = a * 6 + b;
			dButtons[n] = new DataButton(b * 200, a * 100);
			dButtons[n]->StartButton->text += to_string(n);
			if (mapdatas[n]) {
				dButtons[n]->StartButton->text2 = "作成済み";
			}
		}
	}
}
int mapnum = 0;
void enemyDataToBox(int datanum) {
	numBox[0]->num = &mapdatas[mapnum]->eDatas[datanum]->x;
	numBox[1]->num = &mapdatas[mapnum]->eDatas[datanum]->y;
	numBox[2]->num = &mapdatas[mapnum]->eDatas[datanum]->movetype;
	numBox[3]->num = &mapdatas[mapnum]->eDatas[datanum]->shottype;
	numBox[4]->num = &mapdatas[mapnum]->eDatas[datanum]->syutugenSec;
	numBox[5]->num = &mapdatas[mapnum]->eDatas[datanum]->deathSec;
	numBox[6]->num = &mapdatas[mapnum]->eDatas[datanum]->size;
	numBox[7]->num = &mapdatas[mapnum]->eDatas[datanum]->img;
}
void enemyChoiceInit(int num) {
	string boxStr[E_DATA_SIZE] = { "X","Y","動きのタイプ","弾のタイプ","出現時間","死亡時間","サイズ","画像" };
	for (int i = 0; i < E_DATA_SIZE; i++) {
		numBox[i] = new NumberInputFlame(100, 20 + i * 100, boxStr[i], 160, 80);
	}
	SecBox = new NumberInputFlame(300, 720, "時間", 200, 100);
	eNumBox = new NumberInputFlame(300, 820, "敵番号", 200, 100, 99);
	SecBox->num = &mapdatas[num]->sec;
	eCreButton = new ClickInputFlame(700, 600, 32, 32, "", 0, 0, img[0]);
	eDelButton = new ClickInputFlame(800, 700, 32, 32, "", 0, 0, img[1]);
	mapnum = num;
}
int scene = 0;

void enemyDataDelBox() {
	for (int i = 0; i < E_DATA_SIZE; i++) {
		numBox[i]->num = nullptr;
	}
}
NumberInputFlame *activeBox;
void boxUpdateDainyu(NumberInputFlame *pnumbox) {
	if (pnumbox&&pnumbox->update(activeBox == pnumbox)) {
		activeBox = pnumbox;
	}

}
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Cursor* cursor = Cursor::getInstance();
	keymouseInput* input = keymouseInput::getInstance();
	SetGraphMode(1200, 800, 16, 60);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);
	img[0] = LoadGraph("../dx_shooting/images/plus.png", FALSE);
	img[1] = LoadGraph("../dx_shooting/images/batu.png", FALSE);
	memset(mapdatas, 0, sizeof(mapdatas));
	GetWindowSize(&winWidth,&winHeight);
	/*FileTest();
	FileTest2();*/
	cursor->cursorImageInit();
	SetFontSize(16);
	t2k::vec3 a = t2k::vec3(10, 20, 30);

	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !input->keyDownCheck() && !input->mouseDownCheck()) {
		GetMousePoint(&cursor->mouseX, &cursor->mouseY);
		//DrawFormatString(cursor->mouseX, cursor->mouseY, 0xffffff, "X=%d,Y=%d", cursor->mouseX, cursor->mouseY);
		cursor->cNum = Cursor::def;
		switch (scene)
		{
		case 0:
			//データ選択画面の初期化
			dataChoiceInit();
			scene = 1;
		case 1:
			DrawFormatString(1200, 900, 0xffffff, "%d", GetFontSize());
			for (int i = 0; i < 30; i++) {
				bool start = false;
				if (dButtons[i]->StartButton->update() && mapdatas[i]) {
					start = true;
				}
				if (dButtons[i]->CreateButton->update()) {
					if (!mapdatas[i]) {
						mapdatas[i] = new mapData();
						dButtons[i]->StartButton->text2 = "作成済み";
						start = false;
					}
				}
				if (dButtons[i]->DeleteButton->update()) {
					if (mapdatas[i]) {
						SAFE_DELETE(mapdatas[i]);
						dButtons[i]->StartButton->text2 = "";
						start = false;
					}
				}
				if (start) {
					enemyChoiceInit(i);
					scene = 2;
				}

			}

			break;
		case 2:

			int n = *eNumBox->num;
			mapdatas[mapnum]->outputData();
			imasec = (float)mapdatas[mapnum]->sec *tumami();
			DrawFormatString(60, 60, 0xffffff, "%f", imasec);
			if (mapdatas[mapnum]->eDatas[n]) {
				for (int i = 0; i < E_DATA_SIZE; i++) {
					boxUpdateDainyu(numBox[i]);
				}
			}
			boxUpdateDainyu(SecBox);
			boxUpdateDainyu(eNumBox);
			if (activeBox) {
				activeBox->plusNum(suutiNyuryoku());
			}
			if (eCreButton->update()) {
				if (!mapdatas[mapnum]->eDatas[n]) {
					mapdatas[mapnum]->eDatas[n] = new ENEMY_DATA;
					enemyDataToBox(n);
				}
			}
			if (eDelButton->update()) {
				if (mapdatas[mapnum]->eDatas[n]) {
					SAFE_DELETE(mapdatas[mapnum]->eDatas[n]);
					enemyDataDelBox();
				}
			}
			if (mapdatas[mapnum]->eDatas[3]) {
				DrawFormatString(0, 0, 0xffffff, "%d", mapdatas[mapnum]->eDatas[3]->x);
			}
			break;
		}
		cursor->cursorDraw();
	}
	DxLib_End();

	return 0;
}
