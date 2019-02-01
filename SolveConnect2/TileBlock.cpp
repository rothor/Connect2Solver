#include "TileBlock.h"


TileBlock::TileBlock() : Tile()
{
	m_type = TileType::block;
}

MoveInstructionsEnter TileBlock::getMoveInstructionsEnter(PathInfoForBoard pathInfo)
{
	return MoveInstructionsEnter(false, false, Direction::up, false);
}

MoveInstructionsExit TileBlock::getMoveInstructionsExit(PathInfoForBoard pathInfo)
{
	return MoveInstructionsExit(false, false, Point(0, 0));
}