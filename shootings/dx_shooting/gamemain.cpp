#include <DxLib.h>
#include <math.h>
#include "keycon.h"
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
#define PI 3.1415926f
keymouseInput input;
int width;
int height;
int mousex;
int mousey;
const int shotNum = 1000;
bool maruHantei(float a, float b, float c) {
	return(a*a + b*b <= c*c);
}
class Debug {
public:
	int objSuu;
	int preTime;
	float dTime;
	int halfTimer;
	int myfps;
	Debug() {
		preTime = 0;
		dTime = 0.06f;
		halfTimer = 0;
		myfps = 0;
	}
	void update(bool showCommand) {
		int nowtime = GetNowCount();
		int satime;
		if (preTime != 0) {
			satime = nowtime - preTime;
			dTime = (double)(satime) / 1000.0;
			halfTimer += satime;
		}
		preTime = nowtime;
		if (halfTimer > 500) {//1/2秒毎の処理
			myfps = 1000 / (satime);
			halfTimer = 0;
		}

		if (showCommand) {
			DrawFormatString(0, 0, GetColor(255, 255, 255), "X=%d Y=%d,\nfps=%d,\ntime=%d,\ndtime=%f\nobj=%d\n", mousex, mousey, myfps, nowtime, dTime, objSuu);
		}

	}
};
Debug* debug;
void drawHp(float startX, float startY, int Hp, int maxHp) {
	float H = (float)Hp / (float)maxHp;
	float tate = 5;
	float yoko = 45;
	DrawBox(startX, startY, startX + yoko, startY + tate, GetColor(255, 0, 0), true);
	DrawBox(startX, startY, startX + H * yoko, startY + tate, GetColor(0, 255, 0), true);

}
class Tama {
public:
	int Attack = 1;
	float Timer = 0;
	float speed;
	float x;
	float y;
	float r;
	int handle = -1;
	int c;
	float angle;
	float size = 1;
	Tama(float X, float Y, float Angle, float Size, int Col, float Spd, int gfx = -1) {
		handle = gfx;
		x = X;
		y = Y;
		r = Size/2;
		size = Size;
		angle = Angle;
		c = Col;
		speed = Spd;
	}
	Tama() {

	}
	void draw() {
		Timer += debug->dTime;
		if (!(handle == -1)) {
			DrawRotaGraphF(x, y, size, angle, handle, TRUE);
		}
		else {
			DrawCircle(x, y, r, c);
		}

	}
};


Tama* pShot[shotNum];
Tama* eShot[shotNum];
bool screenInside(float px, float py, float sabun) {
	if (-sabun <= px && px <= width + sabun && -sabun <= py&&py <= height + sabun) {
		return true;
	}
	else {
		return false;
	}
	return false;
}
class Jet :public Tama {
public:
	int health = 1;
	int maxhealth = 1;
	float AttackSpeed = 1;
	int moveType = 0;
	float x0;
	float y0;	
	float spinTimer = 0;
	float deathTimer = 0;
	Jet(float pX, float pY, float pR, float pAngle, float pSize, float pSpd, float pHealth, float pAs, int gfx = -1) {
		handle = gfx;
		x = pX;
		x0 = pX;
		y = pY;
		y0 = pY;
		r = pR;
		size = pSize;
		angle = pAngle;
		speed = pSpd;
		maxhealth = pHealth;
		health = pHealth;
		AttackSpeed = pAs;
	}
	void eneMove() {
		spinTimer += debug->dTime * 5;
		
		switch (moveType)
		{
		case 0:
			x = x0 + r*cos(spinTimer);
			y = y0 + r*sin(spinTimer);
			break;
		case 1:
			x -= debug->dTime * 10;
			y += debug->dTime * 10;
			break;
		default:
			break;
		}

	};
	void pMove() {
		//動く処理
		speed = 180 * debug->dTime;
		if (CheckHitKey(KEY_INPUT_LSHIFT)) {
			speed *= 2;
		}
		int putkey = 0;
		int mx = 0;
		int my = 0;
		if (CheckHitKey(KEY_INPUT_A) && 0 < x) {
			mx = -1;
			putkey++;
		}
		else if (CheckHitKey(KEY_INPUT_D) && width > x) {
			mx = 1;
			putkey++;
		}
		if (CheckHitKey(KEY_INPUT_W) && 0 < y) {
			my = -1;
			putkey++;
		}
		else if (CheckHitKey(KEY_INPUT_S) && height > y) {
			my = 1;
			putkey++;
		}
		if (putkey >= 2) {
			speed *= 0.7;
		}
		x += mx*speed;
		y += my*speed;
	}
	//弾のポインタをくれれば一つ作れる。
	void shotGen(int px, int py, Tama* shot[], int col, float spd) {
		float xsa = (px - x);
		float ysa = (py - y);
		angle = atan2(ysa, xsa);
		for (int i = 0; i < shotNum; i++) {
			if (shot[i] == nullptr) {
				shot[i] = new Tama(x, y, 4, angle, 1, col, spd);
				Timer = 0;
				break;
			}
		}
	}
	void drawJet() {
		debug->objSuu++;
		draw();
		drawHp(x - r, y - r, health, maxhealth);
	}

};
//自分の弾と貰ったjetが当たってたら消す関数。
void drawAndHit(Jet* jets[], Tama* shot[], int loopnum = 1) {
	for (int i = 0; i < shotNum; i++) {
		if (shot[i]) {
			//実態がある時の処理
			if (screenInside(shot[i]->x, shot[i]->y, 20)) {
				//画面内の時の処理
				shot[i]->x += cos(shot[i]->angle) * shot[i]->speed * 100 * debug->dTime;
				shot[i]->y += sin(shot[i]->angle) * shot[i]->speed * 100 * debug->dTime;
				debug->objSuu++;
				shot[i]->draw();
				for (int k = 0; k < loopnum; k++) {
					if (jets[k]) {
						if (maruHantei(jets[k]->x - shot[i]->x, jets[k]->y - shot[i]->y, jets[k]->r + shot[i]->r)) {
							jets[k]->health -= shot[i]->Attack;
							SAFE_DELETE(shot[i]);
							if (jets[k]->health <= 0) {
								if (loopnum==1) {
									DxLib_End();
								}
							}
							break;
						}
					}
				}

			}
			else {
				SAFE_DELETE(shot[i]);
			}

		}
	}
}
int img[10];










int tst=100;
typedef struct {
	int x = 0;
	int y = 0;
	int type = 0;
	float sec = 0;
}ENEMY_DATA;
ENEMY_DATA *datas[3];
ENEMY_DATA hyozi;













Jet* myJet;
Jet* enemy[10];
//画面内＋差分にいたらtrueを返す

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetGraphMode(1920, 1080,16,60);
	ChangeWindowMode(FALSE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);//ウィンドウモード変更と初期化と裏画面設定
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	img[0] = LoadGraph("images/title.jpg", false);
	img[1] = LoadGraph("images/mons1.png", false);
	img[2] = LoadGraph("images/mons2.png", false);
	img[3] = LoadGraph("images/mons3.png", false);
	img[4] = LoadGraph("images/mons4.png", false);
	img[5] = LoadGraph("images/mons5.png", false);
	img[6] = LoadGraph("images/mons6.png", false);
	img[7] = LoadGraph("images/Cursor.png",false);
	GetWindowSize(&width, &height);
	//敵機作成
	for (int i = 0; i < 10; i++) {
		int n = (i + 1) % 7 + 1;
		enemy[i] = new Jet(width-50, GetRand(height), 32, 0, 0.5, GetRand(5) + 1, n, 0.3f + GetRand(5.0f) / 5.0f,img[n]);
	}
	//自機作成
	myJet = new Jet(width / 2, height / 2, 10, -PI / 2, 0.5, 10, 3, 0.2,LoadGraph("images/ziki.png", false));
	debug = new Debug;
	memset(eShot, 0, sizeof(eShot));
	//毎フレーム処理
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && input.keyDownCheck() == 0 && input.mouseDownCheck() == 0) {

		debug->objSuu = 0;
		DrawGraph(0, 0, img[0], false);
		myJet->pMove();
		myJet->drawJet();
		GetMousePoint(&mousex, &mousey);
		//クリックで弾を作る処理
		if (GetMouseInput()&MOUSE_INPUT_LEFT&&myJet->Timer >= myJet->AttackSpeed) {
			myJet->shotGen(mousex, mousey, pShot, 0xffffff, 1.5f);
		}
		//敵描画
		for (int s = 0; s < 10; s++) {
			if (enemy[s]) {
				//敵の弾作る。
				if (enemy[s]->AttackSpeed <= enemy[s]->Timer) {
					enemy[s]->shotGen(myJet->x, myJet->y, eShot, GetColor(2, 200, 2), (float)(GetRand(9) + 1) / 10.0f);
				}
				if (enemy[s]->deathTimer != 0 && enemy[s]->Timer >= enemy[s]->deathTimer) {
					SAFE_DELETE(enemy[s]);
				}
				if (maruHantei(enemy[s]->x - myJet->x, enemy[s]->y - myJet->y, enemy[s]->r + myJet->r)) {
					SAFE_DELETE(myJet);
					DxLib_End();
				}
				enemy[s]->eneMove();
				enemy[s]->drawJet();
			}

		}
		//当たり判定と弾描画
		drawAndHit(&myJet, eShot);
		drawAndHit(enemy, pShot, 10);
		for (int i = 0; i < 10; i++) {
			if (enemy[i]) {
				if (enemy[i]->health <= 0) {
					SAFE_DELETE(enemy[i]);
				}
			}
		}
		
		debug->update(true);
		DrawRotaGraph(mousex, mousey, 1.0, 0, img[7], true);
	}
	DxLib_End();
	return 0;
}