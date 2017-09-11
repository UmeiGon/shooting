#pragma once
#include "cursor.h"
#include "debug.h"
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
#define MY_PI 3.141592f
class playscene;
class titlescene;
class resultscene;
class menuscene;
class GameManager {
public:
	Cursor *cursor;
	playscene *scene_play;
	titlescene *scene_title;
	resultscene *scene_result;
	menuscene *scene_menu;
	keymouseInput *input;
	Debug *debug;
	void playInit();
	void playEnd();
	void resultInit(int);
	void resultEnd();
	void menuInit();
	void menuEnd();
	void gameInit();
	static GameManager* getInstance();
	GameManager();
	enum teisu{MAX_UI_SUU=10,MAX_BACKIMG_SUU=10,UI_ZONE=200};
	enum backname{TITLE_B,MENU_B,CLOUD1_B};
	enum uigfx { MIGI, HIDARI ,SHOTFLAME,ROBO_ICON};
	int UIImg[MAX_UI_SUU];
	int backImg[MAX_BACKIMG_SUU];
	enum scene{init,title,play,result,menu}gameScene;
	void drawBar(float startX, float startY,float w, float h,  int value, int maxValue,int col = -1, int fillCol = -1);
	int battleWidth;
	int battleHeight;
	int winWidth;
	int winHeight;
};