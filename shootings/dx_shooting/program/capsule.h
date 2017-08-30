#pragma once
class Capsule {
public:
	t2k::vec3 a;
	t2k::vec3 b;
	int r;
	Capsule(t2k::vec3 A, t2k::vec3 B, int R);
	Capsule();
	static void LINE(t2k::vec3 fa, t2k::vec3 fb, int col);
	static int senbunKyori(Capsule cap1, Capsule cap2);
	static bool kousahantei(Capsule cap1, Capsule cap2);
	static bool capsuleHantei(Capsule cap1, Capsule cap2);
	static bool capmaruHantei(Capsule cap1, t2k::vec3 point,int R);
	static void testCap(Capsule* cap1, Capsule* cap2,t2k::vec3 *point =nullptr,int R=0);
};