#include "RecursionStruct.h"
#include "SolutionInputManager.h"
#include "IdManagerSqlite.h"
#include "IdManagerMemory.h"
#include "MoveInputTreeMemory.h"
#include "MoveInputTreeSqlite.h"
#include "SolvedInterfaceComplete.h"
#include "SolvedInterfaceHash.h"


RecursionStruct::RecursionStruct(Connect2 game, SolveOptions so) :
	game(game),
	gameStart(game),
	movesEvaluated(0),
	movesEvaluatedValid(0),
	solutionFound(false),
	depth(0),
	numBranches(0),
	solveEndPos(so.solveEndPos)
{
	if (so.hashtableInSqlite) {
		idManager = std::unique_ptr<IdManager>{
			new IdManagerSqlite()
		};
	}
	else {
		idManager = std::unique_ptr<IdManager>{
			new IdManagerMemory()
		};
	}
	
	if (so.treeInSqlite) {
		mit = std::unique_ptr<MoveInputTree>{
			new MoveInputTreeSqlite()
		};
	}
	else {
		mit = std::unique_ptr<MoveInputTree>{
			new MoveInputTreeMemory()
		};
	}
	
	if (so.solveToPos) {
		si = std::unique_ptr<SolvedInterface>{
			new SolvedInterfaceHash(so.endHash)
		};
	}
	else {
		si = std::unique_ptr<SolvedInterface>{
			new SolvedInterfaceComplete()
		};
	}
}

void RecursionStruct::addSolution(GameInput gi)
{
	SolutionInputManager sim(gameStart);
	for (auto moveIt = gi.miArr.begin(); moveIt != gi.miArr.end(); moveIt++) {
		sim.addMove(*moveIt);
	}
	// Check that new solution isn't the same as any of the current solutions
	GameInput newGi = sim.getSolution();
	for (auto solution : solutionList) {
		if (newGi.getSaveString() == solution.getSaveString())
			return;
	}
	// If not, add solution
	solutionList.push_back(newGi);
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