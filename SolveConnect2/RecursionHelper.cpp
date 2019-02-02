#include "RecursionHelper.h"


RecursionHelper::RecursionHelper(Connect2 game) :
	movesEvaluated(0),
	movesEvaluatedValid(0),
	movesEvaluatedDepth(-1),
	game(game)
{
	gameInputArr.push_back(GameInput());
	idManager.addIdIsUnique(game.getIdStr());
}

std::string RecursionHelper::outputStr()
{
	std::string retStr = "Moves evaluated: " + std::to_string(movesEvaluated) + "\n" +
		"Moves evaluated valid: " + std::to_string(movesEvaluatedValid) + "\n";
	
	if (solution.miArr.size() == 0) {
		retStr += "Solution not found.";
		return retStr;
	}

	retStr += "Solution (" + std::to_string(solution.miArr.size()) + " moves):";
	retStr += game.getGameInputDisplayStr(solution);

	return retStr;
}

void RecursionHelper::clearAllUnnecessaryData()
{
	gameInputArr.clear();
	idManager.clear();
}