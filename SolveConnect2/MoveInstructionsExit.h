#pragma once
#include "Point.h"


class MoveInstructionsExit
{
public:
	MoveInstructionsExit(bool isForced = false,
		bool mustTeleport = false,
		Point teleportPoint = Point(0, 0),
		bool resetIfOnlyForcedMovesAfter = false);
	
public:
	bool mustTeleport;
	Point teleportPoint;
	bool isForced;
	bool resetIfOnlyForcedMovesAfter;
};
