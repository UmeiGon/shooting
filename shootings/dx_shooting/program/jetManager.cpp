#include "jetManager.h"
#include "gameManager.h"
#include "TamaJet.h"
#include "debug.h"
#include "Animation.h"
#include "Dxlib.h"
JetManager* JetManager::getInstance() {
	static JetManager* instance = nullptr;
	if (nullptr == instance) {
		instance = new JetManager();
	}
	return instance;
}
int JetManager::getGraphYsize(int img) {
	int a, b;
	GetGraphSize(img,&a,&b);
	return b;
}
JetManager::JetManager() {
	
	memset(anims, 0, sizeof(anims));
	memset(enemy, 0, sizeof(enemy));
	memset(targetJet, 0, sizeof(targetJet));
	player = nullptr;
}
void JetManager::animationUpdate() {
	GameManager *gm = GameManager::getInstance();
	for (int i = 0; i < MAX_ANIM_SUU; i++) {
		if (anims[i]) {
			anims[i]->timer += gm->debug->dTime;
			if (anims[i]->img[(int)(anims[i]->timer / anims[i]->changeTime)] == 0) {
				SAFE_DELETE(anims[i]);
				continue;
			}
			DrawRotaGraph(anims[i]->pos.x, anims[i]->pos.y, 1.0, 0, anims[i]->img[(int)(anims[i]->timer /anims[i]->changeTime)], true);
		}
	}
}
void JetManager::animStart(t2k::vec3 Pos,int Img[]) {
	for (int i = 0; i < MAX_ANIM_SUU;i++) {
		if (!anims[i]) {
			anims[i] = new Animation(Pos,Img);
			break;
		}
	}
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
void JetManager::damagesSyori(Jet* shooter) {
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

