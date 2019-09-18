#include "Connect2Solver.h"
#include "Misc.h"
#include "SolutionInputManager.h"
#include "Recursor.h"
#include "Benchmarker.h"
#include <list>
#include <iostream>
#include <fstream>
#include <memory>


void Connect2Solver::solve(Connect2 game, SolveOptions options)
{
	RecursionStruct rs(game, options);
	if (rs.si->isSolved(rs.game)) {
		std::cout << "Already solved.\n";
		return;
	}

	Benchmarker::clearAllTimes();
	Benchmarker::resetTimer("TotalTime");
	
	bool isUnique = rs.idManager->addIdIsUnique(game.getIdStr());
	std::cout << Misc::padStr("Depth", 6) << Misc::padStr("Paths", 15) << Misc::padStr("Evaluated", 17) << "\n";

	#ifdef doBenchmarking
	std::ofstream out("benchmarks.txt"); // for benchmark file
	out.clear(); // for benchmark file
	out << "[Depth - Paths - Evaluated]\n\n"; // for benchmark file
	#endif

	while (true) {
		rs.numBranches = 0;
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
		Recursor::addNextInputs(rs);
		#ifdef doBenchmarking
		Benchmarker::addTime("recurse"); // ## for benchmarking ##
		#endif
		rs.depth++;

		if (rs.depth % 5 == 1 && rs.depth != 1)
			std::cout << "\n";
		else
			std::cout << "\r";
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.depth), 6);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.numBranches), 15);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.movesEvaluated), 17);

		// --- for benchmark file
		#ifdef doBenchmarking
		out << Misc::formatIntWithCommas(rs.depth) << " - " <<
			Misc::formatIntWithCommas(rs.numBranches) << " - " <<
			Misc::formatIntWithCommas(rs.movesEvaluated) << "\n";

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

		if (rs.solutionFound)
			break;
		if (rs.numBranches == 0)
			break;
	}

	#ifdef doBenchmarking
	out.close(); // for benchmark file
	#endif

	std::cout << "\n" << rs.getDisplayStr() << "\n";

	std::ofstream outSolution("solution.txt"); // for solution file
	outSolution.clear(); // for solution file
	outSolution << rs.getDisplayStr() << "\n\n"; // for solution file
	Benchmarker::addTime("TotalTime");
	outSolution << "Time: " << Benchmarker::getStr("TotalTime"); // for solution file
	outSolution.close(); // for solution file
}