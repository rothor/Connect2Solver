#include "MoveInputRow.h"


MoveInputRow::MoveInputRow(int pathId, std::string direction, int rowId) :
	pathId(pathId),
	direction(charToDirection(direction[0])),
	rowId(rowId),
	mi(MoveInput(pathId, this->direction))
{

}

MoveInputRow::MoveInputRow() :
	pathId(1),
	rowId(0),
	mi(MoveInput(pathId, this->direction))
{

}