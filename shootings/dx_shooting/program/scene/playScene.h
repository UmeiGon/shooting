#pragma once
class JetManager;
class playscene {
public:
	enum result { FIGHTING, VICTORY, DEFEAT };
	playscene();
	int update();
	JetManager* jm;
};
