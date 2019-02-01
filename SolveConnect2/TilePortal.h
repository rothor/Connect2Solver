#pragma once
#include "Tile.h"
#include "Point.h"
#include "MoveInstructionsEnter.h"
#include "MoveInstructionsExit.h"
#include "PathInfoForBoard.h"


class TilePortal : public Tile
{
public:
	TilePortal(Point dest);

	MoveInstructionsEnter getMoveInstructionsEnter(PathInfoForBoard pathInfo);
	MoveInstructionsExit getMoveInstructionsExit(PathInfoForBoard pathInfo);

public:
	Point m_dest;
};

