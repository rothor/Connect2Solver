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

	Benchmarker::clearAllTimes();
	Benchmarker::resetTimer("TotalTime");
	
	std::cout << Misc::padStr("Depth", 6) << Misc::padStr("Paths", 15) << Misc::padStr("Evaluated", 17) << "\n";

	#ifdef doBenchmarking
	std::ofstream out("benchmarks.txt"); // for benchmark file
	out.clear(); // for benchmark file
	out << "[Depth - Paths - Evaluated]\n\n"; // for benchmark file
	#endif

	while (true) {
		#ifdef doBenchmarking
		Benchmarker::clearTime("moveAll"); // ## for benchmarking ##
		Benchmarker::clearTime("getGameId"); // ## for benchmarking ##
		Benchmarker::clearTime("idIsUnique"); // ## for benchmarking ##
		Benchmarker::clearTime("getRows"); // ## for benchmarking ##
		Benchmarker::clearTime("nextRow"); // ## for benchmarking ##
		Benchmarker::clearTime("rmvPrevInputs"); // ## for benchmarking ##
		Benchmarker::clearTime("addInput"); // ## for benchmarking ##
		Benchmarker::clearTime("recurse"); // ## for benchmarking ##
		Benchmarker::resetTimer("recurse"); // ## for benchmarking ##
		#endif
		tr.addNextInputs();
		#ifdef doBenchmarking
		Benchmarker::addTime("recurse"); // ## for benchmarking ##
		#endif

		if (tr.getDepth() % 5 == 1 && tr.getDepth() != 1)
			std::cout << "\n";
		else
			std::cout << "\r";
		std::cout << Misc::padStr(Misc::formatIntWithCommas(tr.getDepth()), 6);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(tr.getNumBranches()), 15);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(tr.getNumMovesEvaluated()), 17);
		if (false) {
			std::cout << Misc::padStr(Misc::formatDoubleAsPercent(tr.getNumMovesEvaluatedValid() / (double)tr.getNumMovesEvaluated()), 10);
			std::cout << Misc::padStr(Misc::formatDoubleAsPercent(tr.getNumMovesEvaluatedUnique() / (double)tr.getNumMovesEvaluated()), 10);
		}

		// --- for benchmark file
		#ifdef doBenchmarking
		out << Misc::formatIntWithCommas(tr.getDepth()) << " - " <<
			Misc::formatIntWithCommas(tr.getNumBranches()) << " - " <<
			Misc::formatIntWithCommas(tr.getNumMovesEvaluated()) << "\n";

		out << "This recursion:\n";
		out << Benchmarker::getStr("recurse") << "\n";
		std::list<std::string> pieces{ "moveAll", "getGameId", "idIsUnique",
			"getRows", "nextRow", "rmvPrevInputs", "addInput"};
		out << Benchmarker::getPieChartStr(pieces) << "\n";
		
		out << "Total recursion:\n";
		out << Benchmarker::getTotalStr("recurse") << "\n";
		out << Benchmarker::getPieChartTotalStr(pieces) << "\n";

		
		Benchmarker::addTime("TotalTime"); // ## for benchmarking ##
		Benchmarker::resetTimer("TotalTime"); // ## for benchmarking ##
		out << Benchmarker::getStr("TotalTime") << "\n\n";
		#endif
		// --- for benchmark file

		if (tr.solutionWasFound())
			break;
		if (tr.getNumBranches() == 0)
			break;
	}

	#ifdef doBenchmarking
	out.close(); // for benchmark file
	#endif

	std::cout << "\n" << tr.getDisplayStr() << "\n";

	std::ofstream outSolution("solution.txt"); // for solution file
	outSolution.clear(); // for solution file
	outSolution << tr.getDisplayStr() << "\n\n"; // for solution file
	Benchmarker::addTime("TotalTime");
	outSolution << Benchmarker::getStr("TotalTime"); // for solution file
	outSolution.close(); // for solution file
}