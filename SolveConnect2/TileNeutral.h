#pragma once
#include "Tile.h"
#include "MoveInstructionsEnter.h"
#include "MoveInstructionsExit.h"
#include "PathInfoForBoard.h"


class TileNeutral : public Tile
{
public:
	TileNeutral();
	
	MoveInstructionsEnter getMoveInstructionsEnter(PathInfoForBoard pathInfo);
	MoveInstructionsExit getMoveInstructionsExit(PathInfoForBoard pathInfo);

protected:
	
};
