#include "Animation.h"

Animation::Animation(t2k::vec3 Pos,int Img[],float Size) {
	for (int i = 0; i < sizeof(Img); i++) {
		img[i] = Img[i];
	}
	size = Size;
	changeTime = 0.1f;
	timer = 0;
	pos = Pos;
}