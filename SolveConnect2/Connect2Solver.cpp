#include "Connect2Solver.h"
#include "Misc.h"
#include "SolutionInputManager.h"
#include <list>
#include <iostream>
#include <fstream>

#include "Benchmarker.h"


void Connect2Solver::solve(std::string fileName, GameInput gi)
{
	Benchmarker::clearAllTimes();
	Benchmarker::resetTimer("TotalTime"); // ## for benchmarking ##
	Connect2 game(fileName);
	RecursionStruct rs(game);
	rs.idManager.beginQuerying();
	bool isUnique = rs.idManager.addIdIsUnique(game.getIdStr());
	rs.idManager.endQuerying();
	rs.gim.beginQuerying();
	rs.gim.addGameInput(gi);
	rs.gim.endQuerying();

	std::ofstream out("benchmarks.txt"); // for benchmark file
	out.clear(); // for benchmark file
	out << "[Depth - Paths - Evaluated]\n\n"; // for benchmark file
	std::cout << Misc::padStr("Depth", 6) << Misc::padStr("Paths", 15) << Misc::padStr("Evaluated", 17) << "\n";
	while (true) {
		rs.numBranches = 0;
		Benchmarker::clearTime("moveAll"); // ## for benchmarking ##
		Benchmarker::clearTime("getGameId"); // ## for benchmarking ##
		Benchmarker::clearTime("idIsUnique"); // ## for benchmarking ##
		Benchmarker::clearTime("getRows"); // ## for benchmarking ##
		Benchmarker::clearTime("nextRow"); // ## for benchmarking ##
		Benchmarker::clearTime("rmvPrevInputs"); // ## for benchmarking ##
		Benchmarker::clearTime("addInput"); // ## for benchmarking ##
		Benchmarker::clearTime("recurse"); // ## for benchmarking ##
		Benchmarker::resetTimer("recurse"); // ## for benchmarking ##
		recurse(rs);
		Benchmarker::addTime("recurse"); // ## for benchmarking ##
		rs.depth++;

		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.depth), 6);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.numBranches), 15);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.movesEvaluated), 17) << "\n";

		// --- for benchmark file
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
		// --- for benchmark file

		if (rs.solutionFound)
			break;
		if (rs.numBranches == 0)
			break;
	}
	out.close(); // for benchmark file
	for (int i = 0; i < gi.miArr.size(); i++) {
		rs.solution.miArr.pop_front();
	}
	if (gi.miArr.size() == 0) {
		game.reset();
		SolutionInputManager sim(game);
		for (auto it = rs.solution.miArr.begin(); it != rs.solution.miArr.end(); it++) {
			sim.addMove(*it);
		}
		rs.solution = sim.getSolution();
	}
	std::cout << rs.getDisplayStr() << "\n";

	out.open("solution.txt"); // for solution file
	out.clear(); // for solution file
	out << rs.getDisplayStr() << "\n"; // for solution file
	out.close(); // for solution file
}

void Connect2Solver::recurse(RecursionStruct& rs)
{
	rs.gim.beginQuerying();
	rs.idManager.beginQuerying();

	Benchmarker::resetTimer("getRows"); // ## for benchmarking ##
	rs.gim.getRows();
	Benchmarker::addTime("getRows"); // ## for benchmarking ##
	GameInput gi;
	while (true) {
		Benchmarker::resetTimer("nextRow"); // ## for benchmarking ##
		bool anotherRowExists = rs.gim.nextRow(gi);
		Benchmarker::addTime("nextRow"); // ## for benchmarking ##
		if (!anotherRowExists)
			break;
		addValidMoves(rs, gi);

		if (rs.solutionFound)
			break;
	}
	Benchmarker::resetTimer("rmvPrevInputs"); // ## for benchmarking ##
	rs.gim.removePrevGameInputs();
	Benchmarker::addTime("rmvPrevInputs"); // ## for benchmarking ##
	
	rs.gim.endQuerying();
	rs.idManager.endQuerying();
}

void Connect2Solver::addValidMoves(RecursionStruct& rs, GameInput& gi)
{
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	rs.game.reset();
	rs.game.moveAll(gi);
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	
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
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	bool moveIsValid = rs.game.move(mi);
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	rs.movesEvaluated++; // for display

	if (!moveIsValid)
		return;
	rs.movesEvaluatedValid++; // for display

	Benchmarker::resetTimer("getGameId"); // ## for benchmarking ##
	std::string id = rs.game.getIdStr();
	Benchmarker::addTime("getGameId"); // ## for benchmarking ##
	Benchmarker::resetTimer("idIsUnique"); // ## for benchmarking ##
	bool isUnique = rs.idManager.addIdIsUnique(id);
	Benchmarker::addTime("idIsUnique"); // ## for benchmarking ##
	if (!isUnique) { // If game id already exists
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		rs.game.undo(); // Only undo if the move was valid.
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		return;
	}
	
	bool isSolved = rs.game.isSolved();
	if (isSolved) {
		rs.solutionFound = true;
		rs.solution = gi;
		rs.solution.miArr.push_back(mi);
		return;
	}

	gi.miArr.push_back(mi);
	Benchmarker::resetTimer("addInput"); // ## for benchmarking ##
	rs.gim.addGameInput(gi);
	Benchmarker::addTime("addInput"); // ## for benchmarking ##
	rs.numBranches++; // for display
	gi.miArr.pop_back();
	
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	rs.game.undo(); // Only undo if the move was valid.
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
}

//##############################################################################################

void Connect2Solver::solveCustom(std::string fileName, GameInput gi, std::string endHash)
{
	Benchmarker::clearAllTimes();
	Benchmarker::resetTimer("total"); // ## for benchmarking ##
	Connect2 game(fileName);
	RecursionStruct rs(game);
	rs.idManager.beginQuerying();
	bool isUnique = rs.idManager.addIdIsUnique(game.getIdStr());
	rs.idManager.endQuerying();
	rs.gim.beginQuerying();
	rs.gim.addGameInput(gi);
	rs.gim.endQuerying();

	std::ofstream out("benchmarks.txt"); // for benchmark file
	out.clear(); // for benchmark file
	out << "[Depth - Paths - Evaluated]\n\n"; // for benchmark file
	std::cout << Misc::padStr("Depth", 6) << Misc::padStr("Paths", 15) << Misc::padStr("Evaluated", 17) << "\n";
	while (true) {
		rs.numBranches = 0;
		Benchmarker::clearTime("moveAll"); // ## for benchmarking ##
		Benchmarker::clearTime("getGameId"); // ## for benchmarking ##
		Benchmarker::clearTime("idIsUnique"); // ## for benchmarking ##
		Benchmarker::clearTime("getRows"); // ## for benchmarking ##
		Benchmarker::clearTime("nextRow"); // ## for benchmarking ##
		Benchmarker::clearTime("rmvPrevInputs"); // ## for benchmarking ##
		Benchmarker::clearTime("addInput"); // ## for benchmarking ##
		Benchmarker::clearTime("recurse"); // ## for benchmarking ##
		Benchmarker::resetTimer("recurse"); // ## for benchmarking ##
		recurseCustom(rs, endHash);
		Benchmarker::addTime("recurse"); // ## for benchmarking ##
		rs.depth++;

		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.depth), 6);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.numBranches), 15);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.movesEvaluated), 17) << "\n";
		
		// --- for benchmark file
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
		// --- for benchmark file

		if (rs.solutionFound)
			break;
		if (rs.numBranches == 0)
			break;
	}
	out.close(); // for benchmark file
	for (int i = 0; i < gi.miArr.size(); i++) {
		rs.solution.miArr.pop_front();
	}
	std::cout << rs.getDisplayStr() << "\n";

	out.open("solution.txt"); // for solution file
	out.clear(); // for solution file
	out << rs.getDisplayStr() << "\n"; // for solution file
	out.close(); // for solution file
}

void Connect2Solver::recurseCustom(RecursionStruct& rs, std::string& endHash)
{
	rs.gim.beginQuerying();
	rs.idManager.beginQuerying();

	Benchmarker::resetTimer("getRows"); // ## for benchmarking ##
	rs.gim.getRows();
	Benchmarker::addTime("getRows"); // ## for benchmarking ##
	GameInput gi;
	while (true) {
		Benchmarker::resetTimer("nextRow"); // ## for benchmarking ##
		bool anotherRowExists = rs.gim.nextRow(gi);
		Benchmarker::addTime("nextRow"); // ## for benchmarking ##
		if (!anotherRowExists)
			break;
		addValidMovesCustom(rs, gi, endHash);

		if (rs.solutionFound)
			break;
	}
	Benchmarker::resetTimer("rmvPrevInputs"); // ## for benchmarking ##
	rs.gim.removePrevGameInputs();
	Benchmarker::addTime("rmvPrevInputs"); // ## for benchmarking ##

	rs.gim.endQuerying();
	rs.idManager.endQuerying();
}

void Connect2Solver::addValidMovesCustom(RecursionStruct& rs, GameInput& gi, std::string& endHash)
{
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	rs.game.reset();
	rs.game.moveAll(gi);
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##

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
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	bool moveIsValid = rs.game.move(mi);
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	rs.movesEvaluated++; // for display

	if (!moveIsValid)
		return;
	rs.movesEvaluatedValid++; // for display

	Benchmarker::resetTimer("getGameId"); // ## for benchmarking ##
	std::string id = rs.game.getIdStr();
	Benchmarker::addTime("getGameId"); // ## for benchmarking ##
	Benchmarker::resetTimer("idIsUnique"); // ## for benchmarking ##
	bool isUnique = rs.idManager.addIdIsUnique(id);
	Benchmarker::addTime("idIsUnique"); // ## for benchmarking ##
	if (!isUnique) { // If game id already exists
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		rs.game.undo(); // Only undo if the move was valid.
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		return;
	}

	bool isSolved = rs.game.getIdStr() == endHash;
	if (isSolved) {
		rs.solutionFound = true;
		rs.solution = gi;
		rs.solution.miArr.push_back(mi);
		return;
	}

	gi.miArr.push_back(mi);
	Benchmarker::resetTimer("addInput"); // ## for benchmarking ##
	rs.gim.addGameInput(gi);
	Benchmarker::addTime("addInput"); // ## for benchmarking ##
	rs.numBranches++; // for display
	gi.miArr.pop_back();

	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	rs.game.undo(); // Only undo if the move was valid.
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
}
