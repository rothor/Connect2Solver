#include "TilePortal.h"


TilePortal::TilePortal(Point dest) : Tile(),
	m_dest(dest)
{
	m_type = TileType::portal;
}

MoveInstructionsEnter TilePortal::getMoveInstructionsEnter(PathInfoForBoard pathInfo)
{
	return MoveInstructionsEnter(true, false, Direction::up, false);
}

MoveInstructionsExit TilePortal::getMoveInstructionsExit(PathInfoForBoard pathInfo)
{
	return MoveInstructionsExit(true, !pathInfo.prevMoveWasTeleport, m_dest, true);
}