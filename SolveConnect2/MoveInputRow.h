#pragma once
#include <string>
#include "Direction.h"
#include "MoveInput.h"


class MoveInputRow
{
public:
	MoveInputRow(int pathId, std::string direction, int rowId);
	MoveInputRow();
	
public:
	int pathId;
	Direction direction;
	int rowId;
	MoveInput mi;
};

