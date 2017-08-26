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
class GameManager {
public:
	Cursor *cursor;
	playscene *scene_play;
	titlescene *scene_title;
	resultscene *scene_result;
	keymouseInput *input;
	Debug *debug;
	void playInit();
	void resultInit(int);
	static GameManager* getInstance();
	GameManager();
	enum scene{init,title,play,result}gameScene;
	int winWidth;
	int winHeight;
};