#include "Connect2Solver.h"
#include "Misc.h"
#include "SolutionInputManager.h"
#include <list>
#include <iostream>
#include <fstream>

#include "Benchmarker.h"


void Connect2Solver::solve(Connect2 game)
{
	Benchmarker::clearAllTimes();
	Benchmarker::resetTimer("TotalTime"); // ## for benchmarking ##
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

		if (rs.depth % 5 == 1 && rs.depth != 1)
			std::cout << "\n";
		else
			std::cout << "\r";
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.depth), 6);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.numBranches), 15);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.movesEvaluated), 17);

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

	std::cout << "\n" << rs.getDisplayStr() << "\n";

	out.open("solution.txt"); // for solution file
	out.clear(); // for solution file
	out << rs.getDisplayStr(); // for solution file
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
	rs.game = rs.gameStart;
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
		GameInput solutionGi = gi;
		solutionGi.miArr.push_back(mi);
		rs.addSolution(solutionGi);
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

void Connect2Solver::solveCustom(Connect2 game, std::string endHash)
{
	Benchmarker::clearAllTimes();
	Benchmarker::resetTimer("total"); // ## for benchmarking ##
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

		if (rs.depth % 5 == 1 && rs.depth != 1)
			std::cout << "\n";
		else
			std::cout << "\r";
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.depth), 6);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.numBranches), 15);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.movesEvaluated), 17);
		
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

	std::cout << "\n" << rs.getDisplayStr() << "\n";

	out.open("solution.txt"); // for solution file
	out.clear(); // for solution file
	out << rs.getDisplayStr(); // for solution file
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
	rs.game = rs.gameStart;
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
		GameInput solutionGi = gi;
		solutionGi.miArr.push_back(mi);
		rs.addSolution(solutionGi);
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

//#####################################################################################

void Connect2Solver::solveEndState(Connect2 game)
{
	/*std::ofstream out2("solution.txt"); // for solution file
	out2.clear(); // for solution file
	out2.close(); // for solution file*/

	std::vector<std::unique_ptr<RecursionStruct>> rsVec;
	for (int i = 0; i < game.getNumOfPaths(); i++) {
		rsVec.push_back(std::unique_ptr<RecursionStruct>(new RecursionStruct(game)));
		solveEndStateOnce(*rsVec[i], i);	
	}
	
	std::list<GameInput> solutionListTemp;
	std::list<GameInput> solutionList;
	solutionList.push_back(GameInput());

	for (int i = 0; i < rsVec.size(); i++) {
		std::cout << "Adding path: " << i + 1 << "\n";
		int total = solutionList.size() * rsVec[i]->solutionList.size();
		int j = 1;
		for (GameInput& gi : solutionList) {
			Connect2 gameStart = game;
			gameStart.moveAll(gi);
			for (auto it2 = rsVec[i]->solutionList.begin(); it2 != rsVec[i]->solutionList.end(); it2++) {
				if (j % (int)pow(10, ceil(log10(j + 1)) - 1) == 0) {
					std::cout << "\r\t" <<
						Misc::padStr(Misc::formatIntWithCommas(j), 13) <<
						" / " <<
						Misc::padStr(Misc::formatIntWithCommas(total), 13);
				}
				j++;
				Connect2 gameStart2 = gameStart;
				gameStart2.moveAll(*it2);
				if (gameStart2.pathIsSolved(i)) {
					GameInput newGi = gi;
					newGi.miArr.insert(newGi.miArr.end(), (*it2).miArr.begin(), (*it2).miArr.end()); // look at this later
					solutionListTemp.push_back(newGi);
				}
			}
		}
		std::cout << "\n";
		solutionList = solutionListTemp;
		solutionListTemp.clear();
	}

	RecursionStruct newRs(game);
	for (GameInput gi : solutionList) {
		newRs.addSolution(gi);
		newRs.solutionFound = true;
	}
	std::cout << newRs.getDisplayStr() << "\n";
	std::ofstream out("solution.txt"); // for solution file
	out.clear(); // for solution file
	out << newRs.getDisplayStr(); // for solution file
	out.close(); // for solution file
}

void Connect2Solver::solveEndStateOnce(RecursionStruct& rs, int pathToSolve)
{
	Benchmarker::clearAllTimes();
	Benchmarker::resetTimer("TotalTime"); // ## for benchmarking ##
	rs.gim.beginQuerying();
	GameInput giTemp;
	rs.gim.addGameInput(giTemp);
	rs.gim.endQuerying();
	if (rs.gameStart.isSolved()) {
		//std::cout << "Already solved.\n";
		return;
	}
	rs.idManager.beginQuerying();
	bool isUnique = rs.idManager.addIdIsUnique(rs.gameStart.getIdStr());
	rs.idManager.endQuerying();

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
		recurseEndState(rs, pathToSolve);
		Benchmarker::addTime("recurse"); // ## for benchmarking ##
		rs.depth++;

		if (rs.depth % 10 == 1 && rs.depth != 1)
			std::cout << "\n";
		else
			std::cout << "\r";
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.depth), 6);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.numBranches), 15);
		std::cout << Misc::padStr(Misc::formatIntWithCommas(rs.movesEvaluated), 17);

		// --- for benchmark file
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
		// --- for benchmark file

		if (rs.numBranches == 0)
			break;
	}
	out.close(); // for benchmark file
	std::cout << "\n";

	//std::cout << "\n" << rs.getDisplayStr() << "\n";

	/*out.open("solution.txt", std::ios::app); // for solution file
	//out.clear(); // for solution file
	out << rs.getDisplayStr() << "\n\n\n"; // for solution file
	out.close(); // for solution file*/
}

void Connect2Solver::recurseEndState(RecursionStruct& rs, int pathToSolve)
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
		addValidMovesEndState(rs, gi, pathToSolve);
	}
	Benchmarker::resetTimer("rmvPrevInputs"); // ## for benchmarking ##
	rs.gim.removePrevGameInputs();
	Benchmarker::addTime("rmvPrevInputs"); // ## for benchmarking ##

	rs.gim.endQuerying();
	rs.idManager.endQuerying();
}

void Connect2Solver::addValidMovesEndState(RecursionStruct& rs, GameInput& gi, int pathToSolve)
{
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	rs.game = rs.gameStart;
	rs.game.moveAll(gi);
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##

	std::list<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };
	std::vector<int> pathList{ pathToSolve };
	for (auto it = pathList.begin(); it != pathList.end(); it++) {
		if (rs.game.pathIsFull(*it)) {
			break;
		}
		for (Direction direction : directions) {
			MoveInput newMi = MoveInput(rs.game.getPathDisplayId(*it), direction, true);
			addMoveEndState(rs, gi, newMi);
		}
	}
}

void Connect2Solver::addMoveEndState(RecursionStruct& rs, GameInput& gi, MoveInput& mi)
{
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	bool moveIsValid = rs.game.moveOnlyDo(mi);
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	rs.movesEvaluated++; // for display

	if (!moveIsValid)
		return;

	int pathId = rs.game.getPathIdFromDisplayId(mi.pathId);
	if ((rs.game.pathIsFull(pathId) && !rs.game.pathIsSolved(pathId)) ||
		(!rs.game.portalsExist() && !rs.game.pathCanBeSolved(pathId))) {
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		rs.game.undo(); // Only undo if the move was valid.
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		return;
	}

	rs.movesEvaluatedValid++; // for display

	Benchmarker::resetTimer("getGameId"); // ## for benchmarking ##
	std::string id = rs.game.getIdStr();
	Benchmarker::addTime("getGameId"); // ## for benchmarking ##
	Benchmarker::resetTimer("idIsUnique"); // ## for benchmarking ##
	bool isUnique = true; //rs.idManager.addIdIsUnique(id);
	Benchmarker::addTime("idIsUnique"); // ## for benchmarking ##
	if (!isUnique) { // If game id already exists
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		rs.game.undo(); // Only undo if the move was valid.
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		return;
	}

	bool isSolved = rs.game.pathIsSolved(pathId);
	if (isSolved) {
		rs.solutionFound = true;
		GameInput solutionGi = gi;
		solutionGi.miArr.push_back(mi);
		rs.addSolution(solutionGi);
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