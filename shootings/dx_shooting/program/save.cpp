#include <stdio.h>
#include "save.h"
#include "jetManager.h"
#include "rune.h"
#include "gameManager.h"
saveData::saveData() {

}
void saveData::save(int saveN) {
	FILE* fp;
	switch (saveN)
	{
	case 1:
		fp = fopen("savedata1.dat", "wb");
		break;
	case 2:
		fp = fopen("savedata2.dat", "wb");
		break;
	case 3:
		fp = fopen("savedata3.dat", "wb");
		break;
	default:
		break;
	}
	JetManager *jm = JetManager::getInstance();
	for (int i = 0; i < PlayerJet::MAX_RUNE_SUU; i++) {
		if (jm->player->runes[i]) {
			runes[i] = *jm->player->runes[i];
		}
		else {
			runes[i].kouka = -1;
			runes[i].type = -1;
		}
	}
	for (int i = 0; i < PlayerJet::SET_RUNE_SUU; i++) {
		if (jm->player->setRunes[i]) {
			setRunes[i] = *jm->player->setRunes[i];
		}
		else {
			setRunes[i].kouka = -1;
			setRunes[i].type = -1;
		}
	}

	for (int i = 0; i < PlayerJet::MAX_ARM_SUU; i++) {
		for (int k = 0; k < PlayerJet::MAX_SHOT_TYPE; k++) {
			weaponflag[i][k] = false;
			if (jm->player->shotData[i][k]) {
				weaponflag[i][k] = true;
			}
		}
	}
	ironkazu = jm->player->ironsuu;
	fwrite((saveData*)this, sizeof(saveData), 1, fp);
	fclose(fp);
}
void saveData::load(int saveN) {
	FILE* fp;
	switch (saveN)
	{
	case 1:
		fp = fopen("savedata1.dat", "rb");
		break;
	case 2:
		fp = fopen("savedata2.dat", "rb");
		break;
	case 3:
		fp = fopen("savedata3.dat", "rb");
		break;
	default:
		break;
	}
	if (fp) {
		fread((saveData*)this, sizeof(saveData), 1, fp);
	}
	JetManager *jm = JetManager::getInstance();
	for (int i = 0; i < PlayerJet::MAX_RUNE_SUU; i++) {
		jm->player->runes[i] = new Rune;
		if (runes[i].kouka != -1) {
			*jm->player->runes[i] = runes[i];
		}
		else {
			SAFE_DELETE(jm->player->runes[i]);
		}
	}
	for (int i = 0; i < PlayerJet::SET_RUNE_SUU; i++) {
		jm->player->setRunes[i] = new Rune;
		if (setRunes[i].kouka != -1) {
			*jm->player->setRunes[i] = setRunes[i];
		}
		else {
			SAFE_DELETE(jm->player->setRunes[i]);
		}
	}

	for (int i = 0; i < PlayerJet::MAX_ARM_SUU; i++) {
		for (int k = 0; k < PlayerJet::MAX_SHOT_TYPE; k++) {
			if (weaponflag[i][k]) {
				jm->player->shotData[i][k]=jm->player->shotDataSet((PlayerJet::armtype)i, (PlayerJet::pShotType)k);
			}
		}
	}
	jm->player->ironsuu = ironkazu;
	fclose(fp);
}