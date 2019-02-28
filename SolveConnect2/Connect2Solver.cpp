#include "Connect2Solver.h"
#include <Windows.h>
#include <list>
#include <iostream>

#include "Timer.h"
#include "HighResolutionTimer.h"


Connect2Solver::Connect2Solver()
{
	
}

std::string formatIntWithCommas(int v)
{
	std::string str = std::to_string(v);
	int numDigits = floor(log(v) / log(10)) + 1;
	int numCommas = numDigits / 3;
	int numLeftDigits = numDigits % 3;
	if (numLeftDigits == 0) {
		numCommas--;
		numLeftDigits = 3;
	}

	std::string ret;
	for (int i = 0; i < numLeftDigits; i++) { // Left-most digits
		ret += str[i];
	}
	for (int i = 0; i < numCommas; i++) { // Every group of 3 after that
		ret += ",";
		ret += str.substr(numLeftDigits + 3 * i, 3);
	}
	return ret;
}

void Connect2Solver::solve(RecursionStruct& rs)
{
	HighResolutionTimer timer; // ##### for timing #####
	timer.reset(); // ##### for timing #####
	while (true) {
		rs.numBranches = 0;
		recurse(rs);
		rs.depth++;

		std::cout << "Depth: " << rs.depth << "\n";
		std::cout << "Paths: " << formatIntWithCommas(rs.numBranches) << "\n";
		std::cout << "Evaluated: " << formatIntWithCommas(rs.movesEvaluated) << "\n";

		if (rs.solutionFound)
			break;
	}
	Timer::addTimeTotal(timer.readMicro()); // ##### for timing #####
	cleanup(rs);
}

void Connect2Solver::recurse(RecursionStruct& rs)
{
	rs.gim->beginQuerying();
	rs.idManager->beginQuerying();

	rs.gim->getRows();
	GameInput gi;
	while (rs.gim->nextRow(gi)) {
		addValidMoves(rs, gi);

		if (rs.solutionFound)
			break;
	}
	rs.gim->removePrevGameInputs();

	rs.gim->endQuerying();
	rs.idManager->endQuerying();
}

void Connect2Solver::addValidMoves(RecursionStruct& rs, GameInput& gi)
{
	HighResolutionTimer timer; // ##### for timing #####
	timer.reset(); // ##### for timing #####
	rs.game.reset();
	rs.game.moveAll(gi);
	Timer::addTime(std::string("MoveAll"), timer.readMicro()); // ##### for timing #####
	
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
	HighResolutionTimer timer; // ##### for timing #####
	timer.reset(); // ##### for timing #####
	bool moveIsValid = rs.game.move(mi);
	rs.movesEvaluated++; // for display
	Timer::addTime(std::string("MoveAll"), timer.readMicro()); // ##### for timing #####

	if (!moveIsValid)
		return;
	rs.movesEvaluatedValid++; // for display

	std::string id = rs.game.getIdStr();
	bool isUnique = rs.idManager->addIdIsUnique(id);
	if (!isUnique) { // If game id already exists
		timer.reset(); // ##### for timing #####
		rs.game.undo(); // Only undo if the move was valid.
		Timer::addTime(std::string("MoveAll"), timer.readMicro()); // ##### for timing #####
		return;
	}

	if (rs.game.isSolved()) {
		rs.solutionFound = true;
		rs.solution = gi;
		rs.solution.miArr.push_back(mi);
		return;
	}

	gi.miArr.push_back(mi);
	rs.gim->addGameInput(gi);
	rs.numBranches++; // for display
	gi.miArr.pop_back();
	
	timer.reset(); // ##### for timing #####
	rs.game.undo(); // Only undo if the move was valid.
	Timer::addTime(std::string("MoveAll"), timer.readMicro()); // ##### for timing #####
}

void Connect2Solver::cleanup(RecursionStruct& rs)
{
	rs.idManager->clear();
	delete rs.idManager;
	rs.gim->clear();
	delete rs.gim;
}