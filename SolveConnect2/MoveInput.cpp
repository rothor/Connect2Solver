#include "MoveInput.h"
#include "Globals.h"


MoveInput::MoveInput(int pathId, Direction direction, bool singleStep) :
	pathId(pathId),
	direction(direction),
	singleStep(singleStep)
{

}

MoveInput::MoveInput(std::string saveStr)
{
	if (saveStr.size() == 2) {
		pathId = std::stoi(saveStr.substr(0, 1));
		direction = charToDirection(saveStr[1]);
		singleStep = false;
	}
	else if (saveStr.size() == 3) {
		pathId = std::stoi(saveStr.substr(0, 1));
		direction = charToDirection(saveStr[1]);
		if (saveStr[2] == Globals::singleMoveChar)
			singleStep = true;
		else
			singleStep = false;
	}
	else
		; // program error
}

MoveInput::MoveInput() :
	pathId(0),
	direction(Direction::up),
	singleStep(false)
{

}

std::string MoveInput::getSaveStr()
{
	if (singleStep)
		return std::string(std::to_string(pathId) + directionToChar(direction) + Globals::singleMoveChar);
	else
		return std::string(std::to_string(pathId) + directionToChar(direction));
}
