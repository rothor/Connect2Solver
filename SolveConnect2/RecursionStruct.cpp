#include "RecursionStruct.h"
#include "SolutionInputManager.h"


RecursionStruct::RecursionStruct(Connect2 game) :
	game(game),
	gameStart(game),
	movesEvaluated(0),
	movesEvaluatedValid(0),
	solutionFound(false),
	depth(0),
	numBranches(0)
{
	
}

void RecursionStruct::addSolution(GameInput gi)
{
	SolutionInputManager sim(gameStart);
	for (auto moveIt = gi.miArr.begin(); moveIt != gi.miArr.end(); moveIt++) {
		sim.addMove(*moveIt);
	}
	solutionList.push_back(sim.getSolution());
}

std::string RecursionStruct::getDisplayStr()
{
	std::string ret;
	if (solutionFound) {
		ret += std::to_string(solutionList.size()) + " solution" + (solutionList.size() > 1 ? "s" : "") + " found in " + std::to_string(solutionList.begin()->miArr.size()) + " moves\n";
		int i = 1;
		for (auto it = solutionList.begin(); it != solutionList.end(); it++) {
			ret += "Solution " + std::to_string(i++) + ":\n";
			ret += it->getDisplayStr() + "\n";
			ret += "Save string: " + it->getSaveString();
			if (std::distance(it, solutionList.end()) > 1)
				ret += "\n\n";
		}
	}
	else {
		ret += "Solution not found.";
	}
	
	return ret;
}