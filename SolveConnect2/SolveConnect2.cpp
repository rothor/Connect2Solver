#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Connect2.h"
#include "RecursionHelper.h"
#include "Direction.h"
#include "MoveInput.h"


void solve(std::string fileName);
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
		std::cout << "---------------------------------------\n";
		if (input[0] == char('p'))
			play(fileName);
		else if (input[0] == char('s'))
			solve(fileName);
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

			game.moveUserInput(mi);
		}
	}
	std::cout << "---------------------------------------\n";
}

void solveRecursion(RecursionHelper& m)
{
	while (true) {
		m.movesEvaluatedDepth++; // for display
		std::cout << "Depth: " << m.movesEvaluatedDepth << "\n";
		std::cout << "Paths: " << m.moveInputArrArr.size() << "\n";
		int inputsSize = m.moveInputArrArr.size();
		if (inputsSize == 0) { // This means no more paths.
			m.moveInputArrArr.clear();
			return;
		}
		int count = 0;
		for (std::list<MoveInput>& miArr : m.moveInputArrArr) {
			count++;
			if (count > inputsSize)
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
					m.movesEvaluated++; // for display
					if (moveIsValid) {
						m.movesEvaluatedValid++; // for display

						// Check that board state is unique
						std::string id = m.game.getIdStr();
						bool isUnique = true;
						auto ret = m.gameStateId.insert(std::pair<std::string, bool>(id, false));
						if (!ret.second) // If key already existed
							continue;

						// If we made it this far then move is valid and leads to a unique board state.
						std::list<MoveInput> newMiArr = miArr;
						newMiArr.push_back(newMi);
						if (m.game.isSolved()) { // If we found a solution...
							m.solution = newMiArr;
							m.moveInputArrArr.clear();
							return;
						}

						m.moveInputArrArr.push_back(newMiArr);
					}
				}
			}
		}
		
		for (int i = 0; i < inputsSize; i++) {
			m.moveInputArrArr.pop_front();
		}
	}
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
		std::cout << "---------------------------------------\n";
		if (i[0] == char('q'))
			return;
	}
}
