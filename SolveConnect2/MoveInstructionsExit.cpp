#include "MoveInstructionsExit.h"


MoveInstructionsExit::MoveInstructionsExit(bool isForced,
	bool mustTeleport,
	Point teleportPoint,
	bool resetIfOnlyForcedMovesAfter) :
	isForced(isForced),
	mustTeleport(mustTeleport),
	teleportPoint(teleportPoint),
	resetIfOnlyForcedMovesAfter(resetIfOnlyForcedMovesAfter)
{

}
