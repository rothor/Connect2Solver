#include "RecursionHelper.h"


RecursionHelper::RecursionHelper(Connect2 game) :
	movesEvaluated(0),
	movesEvaluatedValid(0),
	movesEvaluatedDepth(1),
	movesEvaluatedMaxDepth(1),
	game(game)
{
	gameArr.push_back(game);
	//gameId.push_back(game.getIdStr());
	gameId.insert(std::pair<std::string, int>(game.getIdStr(), 1));
	solution.push_back(std::list<MoveInput>(0));
}

RecursionHelper::~RecursionHelper()
{
	
}

std::string RecursionHelper::outputStr()
{
	std::string retStr = "Moves evaluated: " + std::to_string(movesEvaluated) + "\n" +
		"Moves evaluated valid: " + std::to_string(movesEvaluatedValid) + "\n" +
		"Max Depth: " + std::to_string(movesEvaluatedMaxDepth) + "\n" +
		"Solutions found: " + std::to_string(solution.size()) + "\n";

	int i = 0;
	for (std::list<MoveInput>& miArr : solution) {
		retStr += "Solution " + std::to_string(i) + ": ";
		for (MoveInput& mi : miArr) {
			retStr += std::to_string(mi.pathId);
			Direction dir = mi.direction;
			if (dir == Direction::up)
				retStr += "U";
			else if (dir == Direction::down)
				retStr += "D";
			else if (dir == Direction::left)
				retStr += "L";
			else if (dir == Direction::right)
				retStr += "R";
			else
				retStr += "?";
			retStr += " ";
		}
		retStr += "\n";
		i++;
	}

	/*for (int i = 0; i < solution.size(); i++) {
		retStr += "Solution " + std::to_string(i) + ": ";
		for (int j = 0; j < solution[i].size(); j++) {
			retStr += std::to_string(solution[i][j].pathId);
			Direction dir = solution[i][j].direction;
			if (dir == Direction::up)
				retStr += "U";
			else if (dir == Direction::down)
				retStr += "D";
			else if (dir == Direction::left)
				retStr += "L";
			else if (dir == Direction::right)
				retStr += "R";
			else
				retStr += "?";
			retStr += " ";
		}
		retStr += "\n";
	}*/

	return retStr;
}