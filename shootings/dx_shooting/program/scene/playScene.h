#pragma once
class JetManager;
class playscene {
public:
	float playTimer;
	enum result { FIGHTING, VICTORY, DEFEAT };
	playscene();
	int update();
	JetManager* jm;
};
