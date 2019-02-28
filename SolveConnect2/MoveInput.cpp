#include "MoveInput.h"


MoveInput::MoveInput(int pathId, Direction direction) :
	pathId(pathId),
	direction(direction)
{

}

MoveInput::MoveInput(std::string saveStr)
{
	if (saveStr.size() != 2)
		; // program error
	pathId = std::stoi(saveStr.substr(0, 1));
	direction = charToDirection(saveStr[1]);
}

MoveInput::MoveInput() :
	pathId(0),
	direction(Direction::up)
{

}

std::string MoveInput::getSaveStr()
{
	return std::string("" + std::to_string(pathId) + directionToChar(direction));
}
