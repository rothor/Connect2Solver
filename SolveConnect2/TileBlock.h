#pragma once
#include "Tile.h"
#include "MoveInstructionsEnter.h"
#include "MoveInstructionsExit.h"
#include "PathInfoForBoard.h"


class TileBlock : public Tile
{
public:
	TileBlock();

	MoveInstructionsEnter getMoveInstructionsEnter(PathInfoForBoard pathInfo);
	MoveInstructionsExit getMoveInstructionsExit(PathInfoForBoard pathInfo);

public:

};

