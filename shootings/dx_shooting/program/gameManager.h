#pragma once
#include "cursor.h"
#include "debug.h"
#include "keycon.h"
#include <string>
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

#define MY_PI 3.141592f
class playscene;
class titlescene;
class resultscene;
class menuscene;
class saveData;
class GameManager {
public:
	saveData* saved;
	Cursor *cursor;
	playscene *scene_play;
	titlescene *scene_title;
	resultscene *scene_result;
	menuscene *scene_menu;
	keymouseInput *input;
	Debug *debug;
	bool saveflag[3];
	
	void titleInit();
	void titleEnd();
	void playInit(int stageNum);
	void playEnd();
	void resultInit(int);
	void resultEnd();
	void menuInit();
	void menuEnd();
	void gameInit();
	void Save(int num);
	void Load(int num);
	void DeleteSave(int num);
	static GameManager* getInstance();
	GameManager();
	enum teisu{MAX_UI_SUU=22,MAX_BACKIMG_SUU=10,UI_ZONE=200, SAIDAI_DATA = 3};
	enum backname{TITLE_B,MENU_B,CLOUD1_B,CLOUD2_B,GROUND,MOUTAINS};
	enum uigfx { MIGI, HIDARI ,SHOTFLAME,ROBO_ICON,RUNE_FLAME,DUST_BOX,NEWGAME_GFX,LOADGAME_GFX,BLUE_FLAME,EXIT_GFX,DATA1_GFX,DATA2_GFX,DATA3_GFX,POWA,PLAYER_FLAME};
	int UIImg[MAX_UI_SUU];
	int backImg[MAX_BACKIMG_SUU];
	enum scene{init,title,play,result,menu}gameScene;
	void drawBar(float startX, float startY,float w, float h,  int value, int maxValue,int col = -1, int fillCol = -1);
	string runeStr(int t);
	int battleWidth;
	int battleHeight;
	int winWidth;
	int winHeight;
	int nowStage;
};