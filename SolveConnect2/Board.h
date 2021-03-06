#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Tile.h"
#include "TileNeutral.h"
#include "TileBlock.h"
#include "TileMirror.h"
#include "TilePortal.h"
#include "TileStart.h"
#include "TileEnd.h"
#include "Point.h"
#include "MoveInstructions.h"
#include "PathInfoForBoard.h"


class Board
{
public:
	Board(std::string fileName);

	MoveInstructionsExit getMoveInstructionsExit(Point ptStart, PathInfoForBoard pathInfo);
	MoveInstructionsEnter getMoveInstructionsEnter(Point ptDest, PathInfoForBoard pathInfo);
	MoveInstructions getMoveInstructions(Point ptStart, Point ptDest, PathInfoForBoard pathInfo);
	std::shared_ptr<Tile> getTile(int x, int y);
	int getWidth();
	int getHeight();

protected:
	std::shared_ptr<std::vector<std::vector<std::shared_ptr<Tile>>>> m_tileArr;
	int m_width;
	int m_height;
};
