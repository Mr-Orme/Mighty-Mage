#pragma once
#include "Definitions.h"
class Blackboard
{
public:
	Blackboard();
	~Blackboard();
	GAME_VEC  getJump() { return jump; }
	void updateJump(GAME_VEC newPosition);
private:
	GAME_VEC jump;
	GAME_VEC oldPosition;
};

