#pragma once

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
class keymouseInput;
class Debug;
class Cursor;
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
	static GameManager* getInstance();
	GameManager();
	enum teisu{MAX_BACKIMG_SUU=10};
	enum backname{TITLE_B,MENU_B};
	int backImg[MAX_BACKIMG_SUU];
	enum scene{init,title,play,result,menu}gameScene;
	void drawBar(float startX, float startY,float w, float h,  int value, int maxValue,int col = -1, int fillCol = -1);
	int winWidth;
	int winHeight;
};