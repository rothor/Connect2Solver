#include "PathMove.h"


PathMove::PathMove(Point begin, Point dest, Direction directionIn, bool isForcedIn, bool isTeleport) :
	ptBegin(begin),
	ptDest(dest),
	direction(directionIn),
	isForced(isForcedIn),
	isTeleport(isTeleport)
{

}

PathMove::PathMove() :
	ptBegin(Point(0, 0)),
	ptDest(Point(0, 0)),
	direction(Direction::up),
	isForced(false),
	isTeleport(false)
{

}