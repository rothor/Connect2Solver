#include "MoveInstructionsEnter.h"


MoveInstructionsEnter::MoveInstructionsEnter(bool canEnter,
	bool changeDirectionAfterMove,
	Direction newDirection,
	bool resetIfOnlyForcedMovesAfter) :
	canEnter(canEnter),
	changeDirectionAfterMove(changeDirectionAfterMove),
	newDirection(newDirection),
	resetIfOnlyForcedMovesAfter(resetIfOnlyForcedMovesAfter)
{

}
