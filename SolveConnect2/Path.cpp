#include "Path.h"
#include <string>


Path::Path(int id, int maxLength, Point ptStart) :
	m_id(id),
	m_length(0),
	m_maxLength(maxLength),
	m_start(ptStart),
	m_pos(ptStart),
	m_direction(Direction::up),
	m_teleportedLastMove(false),
	m_move(std::vector<PathMove>())
{
	
}

bool Path::isFull()
{
	return m_length == m_maxLength;
}

Point Path::getPos()
{
	return m_pos;
}

bool Path::havePreviousMoves()
{
	return m_move.size() > 0;
}

void Path::doMove(PathMove move)
{
	m_move.push_back(move);
	m_pos = move.ptDest;
	m_direction = move.direction;
	m_length++;
}

PathMove Path::getLastMove()
{
	return m_move.back();
}

void Path::undoLastMove()
{
	m_pos = m_move.back().ptBegin;
	m_move.pop_back();
	m_length--;
	if (m_move.size() > 0)
		m_direction = m_move.back().direction;
}

void Path::setDirection(Direction pdirNew)
{
	m_direction = pdirNew;
}

Point Path::getDestPoint(Direction direction)
{
	if (direction == Direction::down)
		return Point(m_pos.x, m_pos.y - 1);
	else if (direction == Direction::up)
		return Point(m_pos.x, m_pos.y + 1);
	else if (direction == Direction::left)
		return Point(m_pos.x - 1, m_pos.y);
	else
		return Point(m_pos.x + 1, m_pos.y);
}

Point Path::getStartPoint()
{
	return m_start;
}

PathInfoForBoard Path::getInfoForBoard()
{
	bool lastMoveWasTeleport = false;
	if (m_move.size() > 0)
		lastMoveWasTeleport = m_move.back().isTeleport;
	return PathInfoForBoard(m_id, m_length, m_maxLength, m_direction, lastMoveWasTeleport);
}

int Path::getLength()
{
	return m_length;
}

Direction Path::getDirection()
{
	return m_direction;
}

PathMove Path::getMove(int index)
{
	return m_move[index];
}

int Path::getId()
{
	return m_id;
}

std::string Path::getMoveArrStr()
{
	if (m_move.size() == 0)
		return std::string();
	std::string ret;
	Direction dir = m_move[0].direction;
	int count = 1;
	for (int i = 1; i < m_move.size(); i++) {
		if (m_move[i].direction != dir) {
			ret += directionToChar(dir) + std::to_string(count);
			dir = m_move[i].direction;
			count = 1;
		}
		else
			count++;
	}
	ret += directionToChar(dir) + std::to_string(count);
	return ret;
}

void Path::reset()
{
	m_move.clear();
	m_length = 0;
	m_pos = m_start;
}
