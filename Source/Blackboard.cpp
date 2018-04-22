#include "Blackboard.h"



Blackboard::Blackboard()
{
	oldPosition =nullptr;
	jump = { 0,0 };
}

Blackboard::Blackboard(GAME_VEC* position):Blackboard()
{
	oldPosition = position;
}


Blackboard::~Blackboard()
{
}

void Blackboard::updateJump(GAME_VEC newPosition)
{
	if (oldPosition)
	{
		jump = *oldPosition - newPosition;
		*oldPosition = newPosition;
	}
	else
	{
		jump = newPosition;
	}
}
