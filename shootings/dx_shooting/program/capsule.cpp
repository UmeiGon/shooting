#include "cursor.h"
#include "gameManager.h"
#include "Dxlib.h"
#include "vec3.h"
#include "capsule.h"
Capsule::Capsule(t2k::vec3 A, t2k::vec3 B, int R) {
	a = A;
	b = B;
	r = R;
}
Capsule::Capsule() {};


int Capsule::senbunKyori(Capsule cap1, Capsule cap2) {

	t2k::vec3 abVec(cap1.b.x - cap1.a.x, cap1.b.y - cap1.a.y, 0);//線ABのベクトル
	t2k::vec3 abTaniVec = t2k::vec3Normalize(abVec);//線ABの単位ベクトルをノーマライズで求める

	int Bdis = abTaniVec.x*abVec.x + abTaniVec.y*abVec.y;//ABの長さを出す。

	t2k::vec3 P[2]{ cap2.a,cap2.b };
	int kouho[2];
	for (int i = 0; i < 2; i++) {
		t2k::vec3 paVec(P[i].x - cap1.a.x, P[i].y - cap1.a.y, 0);//点Aから点Pのベクトル
		int Sdis = abTaniVec.x*paVec.x + abTaniVec.y*paVec.y;//内積をAの相対距離Sdisとして出す。
		t2k::vec3 S = cap1.a + (abTaniVec*Sdis);//Sの点を求める。
												//垂線の点がAB上にあるかを判定する。
		if (0 < Sdis&&Sdis < Bdis) {
			kouho[i] = t2k::vec3Distance(P[i], S);
			if (P[0] == P[1])return kouho[i];
			continue;
		}
		//無かったら、点からAかBどちらかまでの線の長さを返す
		float ap = t2k::vec3Distance(cap1.a, P[i]);
		float bp = t2k::vec3Distance(cap1.b, P[i]);
		if (ap < bp) {
			kouho[i] = ap;
			if (P[0] == P[1])return kouho[i];
		}
		else {
			kouho[i] = bp;
			if (P[0] == P[1])return kouho[i];
		}

	}
	if (kouho[0] > kouho[1])kouho[0] = kouho[1];
	return kouho[0];
}
bool Capsule::kousahantei(Capsule cap1, Capsule cap2) {
	t2k::vec3 vecCD = cap2.b - cap2.a;
	t2k::vec3 vecAB = cap1.b - cap1.a;
	t2k::vec3 vecHou(-vecAB.y, vecAB.x, 0);
	t2k::vec3 taniVecHou = t2k::vec3Normalize(vecHou);
	float d = -(cap1.a.x*taniVecHou.x + cap1.a.y*taniVecHou.y);//点Dと線ABの距離。
	float t = -(taniVecHou.x*cap2.a.x + taniVecHou.y*cap2.a.y + d) / (taniVecHou.x*vecCD.x + taniVecHou.y*vecCD.y);
	if (!(0 < t&&t <= 1))return false;
	t2k::vec3 kouten(cap2.a.x + vecCD.x*t, cap2.a.y + vecCD.y*t, 0);
	t2k::vec3 kouA = kouten - cap1.a;
	t2k::vec3 kouB = kouten - cap1.b;
	float nai = kouA.x*kouB.x + kouA.y*kouB.y;
	if (0 <= nai)return false;
	return true;
}
bool Capsule::capsuleHantei(Capsule cap1, Capsule cap2)
{

	int n = senbunKyori(cap1, cap2);
	bool kousa=false;
	if (cap2.a != cap2.b) {
		int n2 = senbunKyori(cap2, cap1);
		if (n > n2)n = n2;
		kousa = kousahantei(cap1, cap2);
	}
	if (cap1.r + cap2.r >= n || kousa) {
		return true;
	}
	return false;
}
bool Capsule::capmaruHantei(Capsule cap, t2k::vec3 point,int R) {
	return capsuleHantei(cap, Capsule(point, point, R));
}
void Capsule::testCap(Capsule* cap1, Capsule* cap2,t2k::vec3 *point,int R) {
	GameManager* gm = GameManager::getInstance();
	cap1->a.x = gm->cursor->mouseX;
	cap1->a.y = gm->cursor->mouseY;
	if (CheckHitKey(KEY_INPUT_UP))cap1->b.y--;
	if (CheckHitKey(KEY_INPUT_DOWN))cap1->b.y++;
	if (CheckHitKey(KEY_INPUT_RIGHT))cap1->b.x++;
	if (CheckHitKey(KEY_INPUT_LEFT))cap1->b.x--;
	if (point) {
		if (capmaruHantei(*cap1, *point, R)) {
			DrawString(0, 20, "当たっている", 0xfffffff);
		}
	}
	else if(capsuleHantei(*cap1,*cap2)) {
		DrawString(0, 20, "当たっている", 0xfffffff);
	}
}
