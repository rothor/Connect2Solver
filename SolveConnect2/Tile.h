#pragma once
#include "MoveInstructionsEnter.h"
#include "MoveInstructionsExit.h"
#include "PathInfoForBoard.h"


enum class TileType { none, neutral, block, portal, mirror, start, end };

class Tile
{
public:
	Tile();
	
	virtual MoveInstructionsEnter getMoveInstructionsEnter(PathInfoForBoard pathInfo) = 0;
	virtual MoveInstructionsExit getMoveInstructionsExit(PathInfoForBoard pathInfo) = 0;

public:
	TileType m_type;
};
