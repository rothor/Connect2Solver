#pragma once
#include <string>
#include "Direction.h"


class MoveInput
{
public:
	MoveInput(int pathId, Direction direction);
	MoveInput(std::string saveStr);
	MoveInput();
	std::string getSaveStr();

public:
	Direction direction;
	int pathId;
};