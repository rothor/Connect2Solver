#include "Connect2Solver.h"
#include "Misc.h"
#include "SolutionInputManager.h"
#include <list>
#include <iostream>
#include <fstream>

//#define doBenchmarking
#ifdef doBenchmarking
#include "Benchmarker.h"
#endif


void Connect2Solver::solve(Connect2 game)
{
	#ifdef doBenchmarking
	Benchmarker::clearAllTimes(); // ## for benchmarking ##
	Benchmarker::resetTimer("TotalTime"); // ## for benchmarking ##
	#endif
	RecursionStruct rs(game);
	if (game.isSolved()) {
		std::cout << "Already solved.\n";
		return;
	}
	rs.idManager.beginQuerying();
	bool isUnique = rs.idManager.addIdIsUnique(game.getIdStr());
	rs.idManager.endQuerying();
	rs.gim.beginQuerying();
	GameInput giTemp;
	rs.gim.addGameInput(giTemp);
	rs.gim.endQuerying();

	#ifdef doBenchmarking
	std::ofstream out("benchmarks.txt"); // for benchmark file
	out.clear(); // for benchmark file
	out << "[Depth - Paths - Evaluated]\n\n"; // for benchmark file
	#endif
	std::cout << Misc::padStr("Depth", 6) << Misc::padStr("Paths", 15) << Misc::padStr("Evaluated", 17) << "\n";
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
		recurse(rs);
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
	outSolution << rs.getDisplayStr(); // for solution file
	outSolution.close(); // for solution file
}

void Connect2Solver::recurse(RecursionStruct& rs)
{
	rs.gim.beginQuerying();
	rs.idManager.beginQuerying();

	#ifdef doBenchmarking
	Benchmarker::resetTimer("getRows"); // ## for benchmarking ##
	#endif
	rs.gim.getRows();
	#ifdef doBenchmarking
	Benchmarker::addTime("getRows"); // ## for benchmarking ##
	#endif
	GameInput gi;
	while (true) {
		#ifdef doBenchmarking
		Benchmarker::resetTimer("nextRow"); // ## for benchmarking ##
		#endif
		bool anotherRowExists = rs.gim.nextRow(gi);
		#ifdef doBenchmarking
		Benchmarker::addTime("nextRow"); // ## for benchmarking ##
		#endif
		if (!anotherRowExists)
			break;
		addValidMoves(rs, gi);

		if (rs.solutionFound)
			break;
	}
	#ifdef doBenchmarking
	Benchmarker::resetTimer("rmvPrevInputs"); // ## for benchmarking ##
	#endif
	rs.gim.removePrevGameInputs();
	#ifdef doBenchmarking
	Benchmarker::addTime("rmvPrevInputs"); // ## for benchmarking ##
	#endif
	
	rs.gim.endQuerying();
	rs.idManager.endQuerying();
}

void Connect2Solver::addValidMoves(RecursionStruct& rs, GameInput& gi)
{
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	rs.game = rs.gameStart;
	rs.game.moveAll(gi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
	
	int numOfPaths = rs.game.getNumOfPaths();
	std::list<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };
	for (int i = 0; i < numOfPaths; i++) {
		for (Direction direction : directions) {
			MoveInput newMi = MoveInput(rs.game.getPathDisplayId(i), direction);
			addMove(rs, gi, newMi);

			if (rs.solutionFound)
				return; // We found the solution, so we don't need to do any more computation.
		}
	}
}

void Connect2Solver::addMove(RecursionStruct& rs, GameInput& gi, MoveInput& mi)
{
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	bool moveIsValid = rs.game.move(mi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
	rs.movesEvaluated++; // for display

	if (!moveIsValid)
		return;
	rs.movesEvaluatedValid++; // for display

	#ifdef doBenchmarking
	Benchmarker::resetTimer("getGameId"); // ## for benchmarking ##
	#endif
	std::string id = rs.game.getIdStr();
	#ifdef doBenchmarking
	Benchmarker::addTime("getGameId"); // ## for benchmarking ##
	Benchmarker::resetTimer("idIsUnique"); // ## for benchmarking ##
	#endif
	bool isUnique = rs.idManager.addIdIsUnique(id);
	#ifdef doBenchmarking
	Benchmarker::addTime("idIsUnique"); // ## for benchmarking ##
	#endif
	if (!isUnique) { // If game id already exists
		#ifdef doBenchmarking
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		#endif
		rs.game.undo(); // Only undo if the move was valid.
		#ifdef doBenchmarking
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		#endif
		return;
	}
	
	bool isSolved = rs.game.isSolved();
	if (isSolved) {
		rs.solutionFound = true;
		GameInput solutionGi = gi;
		solutionGi.miArr.push_back(mi);
		rs.addSolution(solutionGi);
		return;
	}

	gi.miArr.push_back(mi);
	#ifdef doBenchmarking
	Benchmarker::resetTimer("addInput"); // ## for benchmarking ##
	#endif
	rs.gim.addGameInput(gi);
	#ifdef doBenchmarking
	Benchmarker::addTime("addInput"); // ## for benchmarking ##
	#endif
	rs.numBranches++; // for display
	gi.miArr.pop_back();
	
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	rs.game.undo(); // Only undo if the move was valid.
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
}

//##############################################################################################

void Connect2Solver::solveCustom(Connect2 game, std::string endHash)
{
	#ifdef doBenchmarking
	Benchmarker::clearAllTimes(); // ## for benchmarking ##
	Benchmarker::resetTimer("total"); // ## for benchmarking ##
	#endif
	RecursionStruct rs(game);
	if (game.getIdStr() == endHash) {
		std::cout << "Already solved.\n";
		return;
	}
	rs.idManager.beginQuerying();
	bool isUnique = rs.idManager.addIdIsUnique(game.getIdStr());
	rs.idManager.endQuerying();
	rs.gim.beginQuerying();
	GameInput giTemp;
	rs.gim.addGameInput(giTemp);
	rs.gim.endQuerying();

	#ifdef doBenchmarking
	std::ofstream out("benchmarks.txt"); // for benchmark file
	out.clear(); // for benchmark file
	out << "[Depth - Paths - Evaluated]\n\n"; // for benchmark file
	#endif
	std::cout << Misc::padStr("Depth", 6) << Misc::padStr("Paths", 15) << Misc::padStr("Evaluated", 17) << "\n";
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
		recurseCustom(rs, endHash);
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
	outSolution << rs.getDisplayStr(); // for solution file
	outSolution.close(); // for solution file
}

void Connect2Solver::recurseCustom(RecursionStruct& rs, std::string& endHash)
{
	rs.gim.beginQuerying();
	rs.idManager.beginQuerying();

	#ifdef doBenchmarking
	Benchmarker::resetTimer("getRows"); // ## for benchmarking ##
	#endif
	rs.gim.getRows();
	#ifdef doBenchmarking
	Benchmarker::addTime("getRows"); // ## for benchmarking ##
	#endif
	GameInput gi;
	while (true) {
		#ifdef doBenchmarking
		Benchmarker::resetTimer("nextRow"); // ## for benchmarking ##
		#endif
		bool anotherRowExists = rs.gim.nextRow(gi);
		#ifdef doBenchmarking
		Benchmarker::addTime("nextRow"); // ## for benchmarking ##
		#endif
		if (!anotherRowExists)
			break;
		addValidMovesCustom(rs, gi, endHash);

		if (rs.solutionFound)
			break;
	}
	#ifdef doBenchmarking
	Benchmarker::resetTimer("rmvPrevInputs"); // ## for benchmarking ##
	#endif
	rs.gim.removePrevGameInputs();
	#ifdef doBenchmarking
	Benchmarker::addTime("rmvPrevInputs"); // ## for benchmarking ##
	#endif

	rs.gim.endQuerying();
	rs.idManager.endQuerying();
}

void Connect2Solver::addValidMovesCustom(RecursionStruct& rs, GameInput& gi, std::string& endHash)
{
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	rs.game = rs.gameStart;
	rs.game.moveAll(gi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif

	int numOfPaths = rs.game.getNumOfPaths();
	std::list<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };
	for (int i = 0; i < numOfPaths; i++) {
		for (Direction direction : directions) {
			MoveInput newMi = MoveInput(rs.game.getPathDisplayId(i), direction);
			addMoveCustom(rs, gi, newMi, endHash);

			if (rs.solutionFound)
				return; // We found the solution, so we don't need to do any more computation.
		}
	}
}

void Connect2Solver::addMoveCustom(RecursionStruct& rs, GameInput& gi, MoveInput& mi, std::string& endHash)
{
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	bool moveIsValid = rs.game.move(mi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
	rs.movesEvaluated++; // for display

	if (!moveIsValid)
		return;
	rs.movesEvaluatedValid++; // for display

	#ifdef doBenchmarking
	Benchmarker::resetTimer("getGameId"); // ## for benchmarking ##
	#endif
	std::string id = rs.game.getIdStr();
	#ifdef doBenchmarking
	Benchmarker::addTime("getGameId"); // ## for benchmarking ##
	Benchmarker::resetTimer("idIsUnique"); // ## for benchmarking ##
	#endif
	bool isUnique = rs.idManager.addIdIsUnique(id);
	#ifdef doBenchmarking
	Benchmarker::addTime("idIsUnique"); // ## for benchmarking ##
	#endif
	if (!isUnique) { // If game id already exists
		#ifdef doBenchmarking
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		#endif
		rs.game.undo(); // Only undo if the move was valid.
		#ifdef doBenchmarking
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		#endif
		return;
	}

	bool isSolved = rs.game.getIdStr() == endHash;
	if (isSolved) {
		rs.solutionFound = true;
		GameInput solutionGi = gi;
		solutionGi.miArr.push_back(mi);
		rs.addSolution(solutionGi);
		return;
	}

	gi.miArr.push_back(mi);
	#ifdef doBenchmarking
	Benchmarker::resetTimer("addInput"); // ## for benchmarking ##
	#endif
	rs.gim.addGameInput(gi);
	#ifdef doBenchmarking
	Benchmarker::addTime("addInput"); // ## for benchmarking ##
	#endif
	rs.numBranches++; // for display
	gi.miArr.pop_back();

	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	rs.game.undo(); // Only undo if the move was valid.
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
}

//#####################################################################################

void Connect2Solver::solveEndState(Connect2 game)
{
	#ifdef doBenchmarking
	Benchmarker::clearAllTimes(); // ## for benchmarking ##
	Benchmarker::resetTimer("TotalTime"); // ## for benchmarking ##
	#endif
	RecursionStruct rs(game);
	if (game.isSolved()) {
		std::cout << "Already solved.\n";
		return;
	}
	rs.idManager.beginQuerying();
	bool isUnique = rs.idManager.addIdIsUnique(game.getIdStr());
	rs.idManager.endQuerying();
	rs.gim.beginQuerying();
	GameInput giTemp;
	rs.gim.addGameInput(giTemp);
	rs.gim.endQuerying();

	#ifdef doBenchmarking
	std::ofstream out("benchmarks.txt"); // for benchmark file
	out.clear(); // for benchmark file
	out << "[Depth - Paths - Evaluated]\n\n"; // for benchmark file
	#endif
	std::cout << Misc::padStr("Depth", 6) << Misc::padStr("Paths", 15) << Misc::padStr("Evaluated", 17) << "\n";
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
		recurseEndState(rs);
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
			"getRows", "nextRow", "rmvPrevInputs", "addInput" };
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
	outSolution << rs.getDisplayStr(); // for solution file
	outSolution.close(); // for solution file
}

void Connect2Solver::recurseEndState(RecursionStruct& rs)
{
	rs.gim.beginQuerying();
	rs.idManager.beginQuerying();

	#ifdef doBenchmarking
	Benchmarker::resetTimer("getRows"); // ## for benchmarking ##
	#endif
	rs.gim.getRows();
	#ifdef doBenchmarking
	Benchmarker::addTime("getRows"); // ## for benchmarking ##
	#endif
	GameInput gi;
	while (true) {
		#ifdef doBenchmarking
		Benchmarker::resetTimer("nextRow"); // ## for benchmarking ##
		#endif
		bool anotherRowExists = rs.gim.nextRow(gi);
		#ifdef doBenchmarking
		Benchmarker::addTime("nextRow"); // ## for benchmarking ##
		#endif
		if (!anotherRowExists)
			break;
		addValidMovesEndState(rs, gi);
	}
	#ifdef doBenchmarking
	Benchmarker::resetTimer("rmvPrevInputs"); // ## for benchmarking ##
	#endif
	rs.gim.removePrevGameInputs();
	#ifdef doBenchmarking
	Benchmarker::addTime("rmvPrevInputs"); // ## for benchmarking ##
	#endif

	rs.gim.endQuerying();
	rs.idManager.endQuerying();
}

void Connect2Solver::addValidMovesEndState(RecursionStruct& rs, GameInput& gi)
{
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	rs.game = rs.gameStart;
	rs.game.moveAll(gi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif

	std::list<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };
	std::vector<int>* pathList = rs.game.getPathIdsOrderedByLength();
	for (auto it = pathList->begin(); it != pathList->end(); it++) {
		if (rs.game.pathIsFull(*it)) {
			continue;
		}
		for (Direction direction : directions) {
			MoveInput newMi = MoveInput(rs.game.getPathDisplayId(*it), direction, true);
			addMoveEndState(rs, gi, newMi);
		}
		break;
	}
}

void Connect2Solver::addMoveEndState(RecursionStruct& rs, GameInput& gi, MoveInput& mi)
{
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	bool moveIsValid = rs.game.moveOnlyDo(mi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
	rs.movesEvaluated++; // for display

	if (!moveIsValid)
		return;

	int pathId = rs.game.getPathIdFromDisplayId(mi.pathId);
	if ((rs.game.pathIsFull(pathId) && !rs.game.pathIsSolved(pathId)) ||
		(!rs.game.portalsExist() && !rs.game.pathCanBeSolved(pathId))) {
		#ifdef doBenchmarking
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		#endif
		rs.game.undo(); // Only undo if the move was valid.
		#ifdef doBenchmarking
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		#endif
		return;
	}

	rs.movesEvaluatedValid++; // for display

	#ifdef doBenchmarking
	Benchmarker::resetTimer("getGameId"); // ## for benchmarking ##
	#endif
	std::string id = rs.game.getIdStr();
	#ifdef doBenchmarking
	Benchmarker::addTime("getGameId"); // ## for benchmarking ##
	Benchmarker::resetTimer("idIsUnique"); // ## for benchmarking ##
	#endif
	bool isUnique = rs.idManager.addIdIsUnique(id);
	#ifdef doBenchmarking
	Benchmarker::addTime("idIsUnique"); // ## for benchmarking ##
	#endif
	if (!isUnique) { // If game id already exists
		#ifdef doBenchmarking
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		#endif
		rs.game.undo(); // Only undo if the move was valid.
		#ifdef doBenchmarking
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		#endif
		return;
	}

	bool isSolved = rs.game.isSolved();
	if (isSolved) {
		rs.solutionFound = true;
		GameInput solutionGi = gi;
		solutionGi.miArr.push_back(mi);
		rs.addSolution(solutionGi);
	}

	gi.miArr.push_back(mi);
	#ifdef doBenchmarking
	Benchmarker::resetTimer("addInput"); // ## for benchmarking ##
	#endif
	rs.gim.addGameInput(gi);
	#ifdef doBenchmarking
	Benchmarker::addTime("addInput"); // ## for benchmarking ##
	#endif
	rs.numBranches++; // for display
	gi.miArr.pop_back();

	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	rs.game.undo(); // Only undo if the move was valid.
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
}