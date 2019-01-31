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
	std::string retStr = "Moves evaluated: " + std::to_string(movesEvaluated) + "\n";
		//"Moves evaluated valid: " + std::to_string(movesEvaluatedValid) + "\n";
	
	if (solution.size() == 0) {
		retStr += "Solution not found.\n";
		return retStr;
	}

	//

	//

	retStr += "Solution (" + std::to_string(solution.size()) + " moves):";
	int prevPathId = -1;
	for (MoveInput& mi : solution) {
		if (mi.pathId != prevPathId)
			retStr += "\n" + std::to_string(game.getPathDisplayId(mi.pathId)) + ": ";
		prevPathId = mi.pathId;
		retStr += directionToChar(mi.direction) + 'A' - 'a';
	}
	retStr += "\n";

	return retStr;
}