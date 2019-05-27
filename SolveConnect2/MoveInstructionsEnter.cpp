#include "MoveInstructionsEnter.h"


MoveInstructionsEnter::MoveInstructionsEnter(bool canEnter,
	bool changeDirectionAfterMove,
	Direction newDirection,
	bool canStop) :
	canEnter(canEnter),
	changeDirectionAfterMove(changeDirectionAfterMove),
	newDirection(newDirection),
	canStop(canStop)
{

}
