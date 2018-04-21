#include "Blackboard.h"



Blackboard::Blackboard()
{
}


Blackboard::~Blackboard()
{
}

void Blackboard::updateJump(GAME_VEC newPosition)
{
	jump = oldPosition - newPosition;
	oldPosition = newPosition;
}
