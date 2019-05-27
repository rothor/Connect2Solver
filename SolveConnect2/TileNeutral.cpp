#include "TileNeutral.h"


TileNeutral::TileNeutral() : Tile()
{
	m_type = TileType::neutral;
}

MoveInstructionsEnter TileNeutral::getMoveInstructionsEnter(PathInfoForBoard pathInfo)
{
	return MoveInstructionsEnter(true, false, Direction::up, true);
}

MoveInstructionsExit TileNeutral::getMoveInstructionsExit(PathInfoForBoard pathInfo)
{
	return MoveInstructionsExit(false, false, Point(0, 0), false);
}