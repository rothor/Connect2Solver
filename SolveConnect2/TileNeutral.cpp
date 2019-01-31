#include "TileNeutral.h"


TileNeutral::TileNeutral() : Tile()
{
	
}

MoveInstructionsEnter TileNeutral::getMoveInstructionsEnter(PathInfoForBoard pathInfo)
{
	return MoveInstructionsEnter(true, false, Direction::up, false);
}

MoveInstructionsExit TileNeutral::getMoveInstructionsExit(PathInfoForBoard pathInfo)
{
	return MoveInstructionsExit(false, false, Point(0, 0));
}