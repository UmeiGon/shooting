#pragma once
#include "vec3.h"
class Animation {
public:
	Animation(t2k::vec3 Pos,int Img[]);
	float timer;
	t2k::vec3 pos;
	float changeTime;
	int img[20]{0};
};