#include "TileBlock.h"


TileBlock::TileBlock() : Tile()
{

}

MoveInstructionsEnter TileBlock::getMoveInstructionsEnter(PathInfoForBoard pathInfo)
{
	return MoveInstructionsEnter(false, false, Direction::up, false);
}

MoveInstructionsExit TileBlock::getMoveInstructionsExit(PathInfoForBoard pathInfo)
{
	return MoveInstructionsExit(false, false, Point(0, 0));
}