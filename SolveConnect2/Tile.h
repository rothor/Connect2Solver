#pragma once
#include "MoveInstructionsEnter.h"
#include "MoveInstructionsExit.h"
#include "PathInfoForBoard.h"


enum class TileType { none, neutral, block, portal, mirror, start, end };

class Tile
{
public:
	Tile();
	~Tile();
	
	TileType getType();
	virtual MoveInstructionsEnter getMoveInstructionsEnter(PathInfoForBoard pathInfo) = 0;
	virtual MoveInstructionsExit getMoveInstructionsExit(PathInfoForBoard pathInfo) = 0;

protected:
	TileType m_type;
};
