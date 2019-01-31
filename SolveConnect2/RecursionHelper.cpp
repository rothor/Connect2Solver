#include "RecursionHelper.h"


RecursionHelper::RecursionHelper(Connect2 game) :
	movesEvaluated(0),
	movesEvaluatedValid(0),
	movesEvaluatedDepth(0),
	game(game)
{
	gameStateId.insert(std::pair<std::string, bool>(game.getIdStr(), false));
	moveInputArrArr.push_back(std::list<MoveInput>(0));
}

RecursionHelper::~RecursionHelper()
{
	
}

std::string RecursionHelper::outputStr()
{
	std::string retStr = "Moves evaluated: " + std::to_string(movesEvaluated) + "\n" +
		"Moves evaluated valid: " + std::to_string(movesEvaluatedValid) + "\n" +
		"Depth: " + std::to_string(movesEvaluatedDepth) + "\n" +
		"Solutions found: " + std::to_string(solution.size()) + "\n";

	int i = 0;
	retStr += "Solution (" + std::to_string(solution.size()) + " moves): ";
	for (MoveInput& mi : solution) {
		retStr += std::to_string(game.getPathDisplayId(mi.pathId));
		retStr += directionToChar(mi.direction);
		retStr += " ";
	}
	retStr += "\n";
	i++;

	return retStr;
}