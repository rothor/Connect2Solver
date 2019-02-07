#pragma once
#include "Direction.h"


class MoveInstructionsEnter
{
public:
	MoveInstructionsEnter(bool canEnter = false,
		bool changeDirectionAfterMove = false,
		Direction newDirection = Direction::up);
	
public:
	bool changeDirectionAfterMove;
	Direction newDirection;
	bool canEnter;
};
