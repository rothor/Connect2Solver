#pragma once
#include "Point.h"
#include "Direction.h"


class PathMove
{
public:
	PathMove(Point begin, Point dest, Direction directionIn, bool isForcedIn, bool isTeleport);
	PathMove();

public:
	Point ptBegin;
	Point ptDest;
	Direction direction;
	bool isForced;
	bool isTeleport;
};