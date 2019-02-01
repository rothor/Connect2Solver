#include "TileEnd.h"


TileEnd::TileEnd(int pathId) : Tile(),
	m_pathId(pathId)
{
	m_type = TileType::end;
}

MoveInstructionsEnter TileEnd::getMoveInstructionsEnter(PathInfoForBoard pathInfo)
{
	bool canEnter = pathInfo.id == m_pathId && pathInfo.length == pathInfo.maxLength - 1;
	return MoveInstructionsEnter(canEnter, false, Direction::up, false);
}

MoveInstructionsExit TileEnd::getMoveInstructionsExit(PathInfoForBoard pathInfo)
{
	return MoveInstructionsExit(false, false, Point(0, 0));
}