#include "TileStart.h"


TileStart::TileStart(int pathId) : Tile(),
	m_pathId(pathId)
{
	m_type = TileType::start;
}

MoveInstructionsEnter TileStart::getMoveInstructionsEnter(PathInfoForBoard pathInfo)
{
	return MoveInstructionsEnter(false, false, Direction::up, false);
}

MoveInstructionsExit TileStart::getMoveInstructionsExit(PathInfoForBoard pathInfo)
{
	return MoveInstructionsExit(false, false, Point(0, 0));
}