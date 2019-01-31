#pragma once
#include "Direction.h"


class MoveInput
{
public:
	MoveInput(int pathId, Direction direction);
	MoveInput();

public:
	Direction direction;
	int pathId;
};