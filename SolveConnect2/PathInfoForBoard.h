#pragma once
#include "Direction.h"


class PathInfoForBoard
{
public:
	PathInfoForBoard(int id, int length, int maxLength, Direction direction, bool prevMoveWasTeleport);
	
public:
	Direction direction;
	int length;
	int maxLength;
	int id;
	bool prevMoveWasTeleport;
};

