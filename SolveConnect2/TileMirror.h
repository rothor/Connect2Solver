#pragma once
#include "Tile.h"
#include "MoveInstructionsEnter.h"
#include "MoveInstructionsExit.h"
#include "PathInfoForBoard.h"


enum class MirrorType { tl, tr, bl, br };

class TileMirror : public Tile
{
public:
	TileMirror(MirrorType type);
	
	MoveInstructionsEnter getMoveInstructionsEnter(PathInfoForBoard pathInfo);
	MoveInstructionsExit getMoveInstructionsExit(PathInfoForBoard pathInfo);

public:
	MirrorType m_mirrorType;
};

