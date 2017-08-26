#include "resultScene.h"
#include "../inputBox.h"
#include "playScene.h"
resultscene::resultscene(int kekka) {
	retryButton = new ClickBox(300, 400, 200, 100, "Retry", 25, 25);
	switch (kekka)
	{
	default:
		break;
	case playscene::VICTORY:
		retryButton->text2 = "Ÿ‚¿I";
		break;
	case playscene::DEFEAT:
		retryButton->text2 = "•‰‚¯";
		break;
	}
}
bool resultscene::update() {
	if (retryButton->update()) {
		return true;
	}
	return false;
}