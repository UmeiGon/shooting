#include "Animation.h"

Animation::Animation(t2k::vec3 Pos,int Img[]) {
	for (int i = 0; i < sizeof(Img); i++) {
		img[i] = Img[i];
	}
	changeTime = 0.1f;
	timer = 0;
	pos = Pos;
}