#include "gameManager.h"
#include "keycon.h"
#include "Debug.h"
#include "cursor.h"
GameManager* GameManager::getInstance() {
	static GameManager* instance=nullptr;
	if (instance == nullptr) {
		instance = new GameManager();
	}
	return instance;
}
GameManager::GameManager() {
	input = keymouseInput::getInstance();
	debug = Debug::getInstance();
	cursor = Cursor::getInstance();
	gameScene = initScene;
}