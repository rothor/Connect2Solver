#include "Connect2Solver.h"
#include "Misc.h"
#include "Recursor.h"
#include "Benchmarker.h"
#include <list>
#include <iostream>
#include <fstream>


void Connect2Solver::solve(Connect2 game, SolveOptions options)
{
	Recursor tr(game, options);
	if (tr.isSolved()) {
		std::cout << "Already solved.\n";
		return;
	}

	Benchmarker bm;
	bm.clearAllTimes();
	bm.resetTimer("TotalTime");
	
	std::cout << Misc::padStr("Depth", 6) << Misc::padStr("Paths", 15) << Misc::padStr("Evaluated", 17) << "\n";

	while (true) {
		tr.addNextInputs();

		// Display to cout
		if (tr.getDepth() % 5 == 1 && tr.getDepth() != 1)
			std::cout << "\n";
		else
			std::cout << "\r";
		std::cout << Misc::padStr(Misc::formatIntWithCommas(tr.getDepth()), 6);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(tr.getNumBranches()), 15);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(tr.getNumMovesEvaluated()), 17);

		if (tr.solutionWasFound())
			break;
		if (tr.getNumBranches() == 0)
			break;
	}

	bm.addTime("TotalTime");
	std::cout << "\n" << tr.getDisplayStr() << "\n";

	// Output solution to file
	std::ofstream outSolution("solution.txt");
	outSolution.clear();
	outSolution << tr.getDisplayStr() << "\n\n";
	outSolution << bm.getStr("TotalTime");
	outSolution.close();
}