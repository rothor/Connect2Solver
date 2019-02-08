#include "GameInputRecursionNodeInfo.h"


GameInputRecursionNodeInfo::GameInputRecursionNodeInfo(Connect2 game) :
	game(game),
	movesEvaluated(0),
	movesEvaluatedValid(0),
	solutionFound(false),
	depth(0),
	numBranches(0)
{
	game.reset();
	idManager.beginQuerying();
	idManager.addIdIsUnique(game.getIdStr());
	idManager.endQuerying();
}

void GameInputRecursionNodeInfo::clearUnnecessaryData()
{
	idManager.clear();
}

std::string GameInputRecursionNodeInfo::getDisplayStr()
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