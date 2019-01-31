#include "MoveInput.h"


MoveInput::MoveInput(int pathId, Direction direction) :
	pathId(pathId),
	direction(direction)
{

}

MoveInput::MoveInput() :
	pathId(0),
	direction(Direction::up)
{

}