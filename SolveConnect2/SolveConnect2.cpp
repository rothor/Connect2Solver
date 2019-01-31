#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Connect2.h"
#include "RecursionHelper.h"
#include "Direction.h"
#include "MoveInput.h"


void solve(std::string fileName);
void solve2(std::string fileName);
void play(std::string fileName);

int main()
{
	std::string fileName = "in.txt";
	Connect2 game(fileName);
	RecursionHelper m(game);

	while (true) {
		std::cout << "Type 'p' to play or 's' to solve: ";
		std::string input;
		std::cin >> input;
		std::cout << "-------------------------------\n";
		if (input[0] == char('p'))
			play(fileName);
		else if (input[0] == char('s'))
			solve2(fileName);
	}
}

void play(std::string fileName)
{
	Connect2 game(fileName);
	while (true) {
		game.reset();
		while (true) {
			std::cout << game.getDisplayStr();
			std::string input;
			std::cin >> input;
			if (input[0] == char('r'))
				break; // reset
			else if (input[0] == char('q'))
				return; // quit

			std::string pathIndexStr = input.substr(0, 1);
			MoveInput mi(input[0] - '0', charToDirection(input[1]));

			game.move(mi);
		}
	}
	std::cout << "-------------------------------\n";
}

void solve2RecursionGame(RecursionHelper& m)
{
	int depth = 0;
	while (true) {
		depth++;
		std::cout << "Depth: " << depth << "\n";
		std::cout << "Paths: " << m.gameArr.size() << "\n";
		int gameSize = m.gameArr.size();
		int count = 0;
		for (auto game : m.gameArr) {
			count++;
			if (count > gameSize)
				break;

			int numOfPaths = game.getNumOfPaths();
			std::vector<Direction> paths{ Direction::down, Direction::up, Direction::left, Direction::right };
			for (int i = 0; i < numOfPaths; i++) {
				for (int j = 0; j < paths.size(); j++) {
					// Evaluate new move
					MoveInput newMi = MoveInput(i, paths[j]);
					Connect2 gameCopy = game;
					bool moveIsValid = gameCopy.move(newMi);
					m.movesEvaluated++; // ### for debugging ###
					if (moveIsValid) {
						m.movesEvaluatedValid++; // ### for debugging ###

						// Check that board state is unique
						/*std::string id = gameCopy.getIdStr();
						bool isUnique = true;
						for (std::string& prevId : m.gameId) {
							if (id.compare(prevId) == 0) {
								isUnique = false;
								break;
							}
						}
						if (isUnique)
							m.gameId.push_back(id);
						else {
							//game.undo();
							continue;
						}*/

						if (gameCopy.isSolved()) { // If we found a solution...
							//m.game = game;
							m.gameArr.clear();
							return;
						}

						m.gameArr.push_back(gameCopy);
						//game.undo();
					}
				}
			}
		}

		for (int i = 0; i < gameSize; i++) {
			m.gameArr.pop_front();
		}
	}
}

void solve2Recursion(RecursionHelper& m)
{
	int depth = 0;
	while (true) {
		depth++;
		std::cout << "Depth: " << depth << "\n";
		std::cout << "Paths: " << m.solution.size() << "\n";
		int solutionSize = m.solution.size();
		int count = 0;
		for (std::list<MoveInput>& miArr : m.solution) {
			count++;
			if (count > solutionSize)
				break;

			int numOfPaths = m.game.getNumOfPaths();
			std::vector<Direction> paths{ Direction::down, Direction::up, Direction::left, Direction::right };
			for (int i = 0; i < numOfPaths; i++) {
				for (int j = 0; j < paths.size(); j++) {
					// Setup board
					m.game.reset();
					for (MoveInput& mi : miArr) {
						m.game.move(mi);
					}

					// Evaluate new move
					MoveInput newMi = MoveInput(i, paths[j]);
					bool moveIsValid = m.game.move(newMi);
					m.movesEvaluated++; // ### for debugging ###
					if (moveIsValid) {
						m.movesEvaluatedValid++; // ### for debugging ###

						// Check that board state is unique
						std::string id = m.game.getIdStr();
						bool isUnique = true;

						/*auto result = std::find(m.gameId.begin(), m.gameId.end(), id);
						if (result != std::end(m.gameId)) {
							isUnique = false;
						}*/
						/*for (std::string& prevId : m.gameId) {
							if (id.compare(prevId) == 0) {
								isUnique = false;
								break;
							}
						}*/
						auto ret = m.gameId.insert(std::pair<std::string, int>(id, 1));
						if (!ret.second) // If key already existed
							isUnique = false;

						if (isUnique)
							; //m.gameId.push_back(id);
						else
							continue;

						std::list<MoveInput> newMove = miArr;
						newMove.push_back(newMi);
						
						if (m.game.isSolved()) { // If we found a solution...
							m.solution.clear();
							m.solution.push_back(newMove);
							return;
						}

						m.solution.push_back(newMove);
					}
				}
			}
		}
		
		for (int i = 0; i < solutionSize; i++) {
			m.solution.pop_front();
		}
	}
}

void solve2(std::string fileName)
{
	while (true) {
		Connect2 game(fileName);
		RecursionHelper m(game);
		solve2Recursion(m);
		std::cout << m.outputStr();
		std::string i;
		std::cin >> i;
		std::cout << "-------------------------------\n";
		if (i[0] == char('q'))
			return;
	}
}

void solveRecursion(RecursionHelper& m)
{
	/*//if (m.movesEvaluated > 10000) // ### for debugging ###
	//	return; // ### for debugging ###

	if (m.gameArr.back().isSolved()) {
		m.solution.push_back(m.move);
		return;
	}
	
	int numOfPaths = m.gameArr.back().getNumOfPaths();
	std::vector<Direction> paths{ Direction::down, Direction::up, Direction::left, Direction::right };
	for (int i = 0; i < numOfPaths; i++) {
		for (int j = 0; j < paths.size(); j++) {
			m.movesEvaluated++; // ### for debugging ###
			MoveInput mi = MoveInput(i, paths[j]);
			Connect2 newBoard = m.gameArr.back();
			if (newBoard.move(mi)) {
				bool matchFound = false;
				//std::string idStr = newBoard.getIdStr();
				for (int k = 0; k < m.gameArr.size() - 1; k++) { //chech if any previous boards match current board
					if (newBoard.matches(m.gameArr[k])) {
						matchFound = true;
						break;
					}
				}
				if (matchFound)
					continue;
				else
					; // m.gameId.push_back(idStr);

				// If we made it this far, then mi is a valid move,
				// and we will go one step further into recursion.
				m.movesEvaluatedValid++; // ### for debugging ###
				m.move.push_back(mi); // push move onto vector
				m.gameArr.push_back(newBoard); // push board onto vector
				m.movesEvaluatedDepth++; // ### for debugging ###
				if (m.movesEvaluatedDepth > m.movesEvaluatedMaxDepth) // ### for debugging ###
					m.movesEvaluatedMaxDepth = m.movesEvaluatedDepth; // ### for debugging ###
				solveRecursion(m);
				m.movesEvaluatedDepth--; // ### for debugging ###
				m.move.pop_back(); // pop move from vector
				m.gameArr.pop_back(); // pop board from vector
			}
		}
	}*/
}

void solve(std::string fileName)
{
	while (true) {
		Connect2 game(fileName);
		RecursionHelper m(game);
		solveRecursion(m);
		std::cout << m.outputStr();
		std::string i;
		std::cin >> i;
		std::cout << "-----------------------------\n";
	}
}