#pragma once
#include "Tile.h"
#include "MoveInstructionsEnter.h"
#include "MoveInstructionsExit.h"
#include "PathInfoForBoard.h"


class TileStart : public Tile
{
public:
	TileStart(int pathId);
	
	MoveInstructionsEnter getMoveInstructionsEnter(PathInfoForBoard pathInfo);
	MoveInstructionsExit getMoveInstructionsExit(PathInfoForBoard pathInfo);

public:
	int m_pathId;
};
