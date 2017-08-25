#include "jetManager.h"
#include "gameManager.h"
#include "TamaJet.h"

JetManager* JetManager::getInstance() {
	static JetManager* instance = nullptr;
	if (nullptr == instance) {
		instance = new JetManager();
	}
	return instance;
}
JetManager::JetManager() {
	memset(enemy, 0, sizeof(enemy));
	memset(targetJet, 0, sizeof(targetJet));
	player = nullptr;
}

//”»’èonly
void JetManager::inToTarget(Jet* target) {
	for (int i = 0; i < MAX_TARGET_SUU; i++) {
		if (!targetJet[i]) {
			targetJet[i] = target;
			return;
		}
	}
}
void JetManager::clearTarget() {
	for (int i = 0; i < MAX_TARGET_SUU; i++) {
		targetJet[i] = nullptr;
	}
}
void JetManager::test2(Jet* shooter) {
	if (!shooter)return;
	for (int s = 0; s < MAX_SHOT_SUU; s++) {
		if (shooter->Shot[s]) {
			shooter->drawMoveShot(s);
			for (int t=0; t < MAX_TARGET_SUU&&targetJet[t]&&shooter->Shot[s]; t++) {
				if (Tama::maruHantei(*targetJet[t], *shooter->Shot[s]) && targetJet[t]->health > 0) {
					targetJet[t]->health -= shooter->Shot[s]->Attack;
					SAFE_DELETE(shooter->Shot[s]);
				}
			}
		}
	}
}

