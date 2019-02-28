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
	PathMove getLastMove();
	void setNewDirection(Direction newDirection);
	Point getDestPoint();
	Point getStartPoint();
	PathInfoForBoard getInfoForBoard();
	int getLength();
	int getMaxLength();
	Direction getDirection();
	Direction getLastMoveDirection();
	PathMove getMove(int index);
	int getId();
	std::string getMoveArrStr();
	void doMove(PathMove move);
	void undoLastMove();
	void reset();

protected:
	std::vector<PathMove> m_move;
	int m_id;
	int m_maxLength;
	int m_length;
	Point m_start;
	Point m_pos;
	Direction m_direction;
	Direction m_newDirection;
	Direction m_lastMoveDirection;
	bool m_teleportedLastMove;
};
