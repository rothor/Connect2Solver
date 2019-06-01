#include "RecursionStruct.h"
#include "SolutionInputManager.h"


int RecursionStruct::instanceCount = 1;

RecursionStruct::RecursionStruct(Connect2 game) :
	game(game),
	gameStart(game),
	movesEvaluated(0),
	movesEvaluatedValid(0),
	solutionFound(false),
	depth(0),
	numBranches(0),
	instance(instanceCount++),
	idManager(instance),
	gim(instance)
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
		for (auto it = solutionList.begin(); it != solutionList.end(); it++) {
			ret += "Solution found (" + std::to_string(it->miArr.size()) + " moves):\n";
			ret += it->getDisplayStr() + "\n";
			ret += "save string: " + it->getSaveString();
			if (std::distance(it, solutionList.end()) > 1)
				ret += "\n\n";
		}	
	}
	else {
		ret += "Solution not found.";
	}
	
	return ret;
}