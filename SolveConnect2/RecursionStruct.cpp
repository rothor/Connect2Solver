#include "RecursionStruct.h"


RecursionStruct::RecursionStruct(Connect2 game) :
	game(game),
	movesEvaluated(0),
	movesEvaluatedValid(0),
	solutionFound(false),
	depth(0),
	numBranches(0)
{
	game.reset();
}

std::string RecursionStruct::getDisplayStr()
{
	std::string ret;
	if (solutionFound) {
		ret += "Solution found (" + std::to_string(solution.miArr.size()) + " moves):\n";
		ret += solution.getDisplayStr();
	}
	else {
		ret += "Solution not found.";
	}
	return ret;
}