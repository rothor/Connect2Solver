#include "Direction.h"


char directionToChar(Direction dir)
{
	if (dir == Direction::left)
		return char('l');
	else if (dir == Direction::right)
		return char('r');
	else if (dir == Direction::up)
		return char('u');
	else
		return char('d');
}

Direction charToDirection(char charIn)
{
	if (charIn == char('l'))
		return Direction::left;
	else if (charIn == char('r'))
		return Direction::right;
	else if (charIn == char('u'))
		return Direction::up;
	else
		return Direction::down;
}