#include "Dxlib.h"
#include "titleScene.h"
#include "../inputBox.h"
#include "../vec3.h"
titlescene::titlescene() {
	startButton = new ClickBox(400, 600, 150, 80, "Start");
}
typedef struct {
	int x1; int y1; int x2; int y2;
}line;
bool titlescene::update() {
	line A{40,70,500,230};
	line B{ 10,50,70,14 };
	double len =
	if (startButton->update()) {
		return true;
	}
	return false;
}