#include "TileMirror.h"


TileMirror::TileMirror(MirrorType type) : Tile(),
	m_mirrorType(type)
{
	m_type = TileType::mirror;
}

MoveInstructionsEnter TileMirror::getMoveInstructionsEnter(PathInfoForBoard pathInfo)
{
	bool canEnter = false;
	Direction direction = Direction::up;

	if (m_mirrorType == MirrorType::tl) {
		if (pathInfo.direction == Direction::right) {
			canEnter = true;
			direction = Direction::up;
		}
		else if (pathInfo.direction == Direction::down) {
			canEnter = true;
			direction = Direction::left;
		}
	}
	else if (m_mirrorType == MirrorType::tr) {
		if (pathInfo.direction == Direction::left) {
			canEnter = true;
			direction = Direction::up;
		}
		else if (pathInfo.direction == Direction::down) {
			canEnter = true;
			direction = Direction::right;
		}
	}
	else if (m_mirrorType == MirrorType::bl) {
		if (pathInfo.direction == Direction::right) {
			canEnter = true;
			direction = Direction::down;
		}
		else if (pathInfo.direction == Direction::up) {
			canEnter = true;
			direction = Direction::left;
		}
	}
	else if (m_mirrorType == MirrorType::br) {
		if (pathInfo.direction == Direction::left) {
			canEnter = true;
			direction = Direction::down;
		}
		else if (pathInfo.direction == Direction::up) {
			canEnter = true;
			direction = Direction::right;
		}
	}

	return MoveInstructionsEnter(canEnter, true, direction, false);
}

MoveInstructionsExit TileMirror::getMoveInstructionsExit(PathInfoForBoard pathInfo)
{
	return MoveInstructionsExit(true, false, Point(0, 0), false);
}