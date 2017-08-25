#pragma once
class Debug {
public:
	int objSuu;
	int preTime;
	int halfTimer;
	int myfps;
	float dTime;
	Debug();
	void update(bool);
	static Debug* getInstance();
};