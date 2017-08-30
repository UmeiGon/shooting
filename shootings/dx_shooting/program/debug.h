#pragma once
class Debug {
public:
	bool showDebug;
	int objSuu;
	int preTime;
	int halfTimer;
	int myfps;
	float dTime;
	Debug();
	void update();
	static Debug* getInstance();
};