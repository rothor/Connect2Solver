#pragma once
#include <vector>
#include "Point.h"
#include "Direction.h"
#include "PathMove.h"
#include "PathInfoForBoard.h"


class Path
{
public:
	Path(int id, int maxLength, Point ptStart);

	bool isFull();
	Point getPos();
	bool havePreviousMoves();
	void doMove(PathMove move);
	PathMove getLastMove();
	void undoLastMove();
	void setDirection(Direction pdirNew);
	Point getDestPoint(Direction direction);
	Point getStartPoint();
	PathInfoForBoard getInfoForBoard();
	int getLength();
	Direction getDirection();
	PathMove getMove(int index);
	int getId();
	std::string getMoveArrStr();
	void reset();

protected:
	std::vector<PathMove> m_move;
	int m_id;
	int m_maxLength;
	int m_length;
	Point m_start;
	Point m_pos;
	Direction m_direction;
	bool m_teleportedLastMove;
};
