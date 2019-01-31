#include "PathInfoForBoard.h"


PathInfoForBoard::PathInfoForBoard(int id, int length, int maxLength, Direction direction, bool prevMoveWasTeleport) :
	id(id),
	length(length),
	maxLength(maxLength),
	direction(direction),
	prevMoveWasTeleport(prevMoveWasTeleport)
{

}

