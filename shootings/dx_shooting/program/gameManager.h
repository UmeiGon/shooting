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
class GameManager {
public:
	Cursor *cursor;
	playscene *scene_play;
	keymouseInput *input;
	Debug *debug;
	static GameManager* getInstance();
	GameManager();
	enum scene{initScene,titleScene,playScene,resultScene}gameScene;
	int winWidth;
	int winHeight;
	int img[10];
};