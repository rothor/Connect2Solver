#pragma once
#include "Direction.h"
#include "Point.h"
#include "MoveInstructionsEnter.h"
#include "MoveInstructionsExit.h"


class MoveInstructions : public MoveInstructionsEnter, public MoveInstructionsExit
{
public:
	MoveInstructions(MoveInstructionsEnter enter, MoveInstructionsExit exit);
	
public:
	
};
