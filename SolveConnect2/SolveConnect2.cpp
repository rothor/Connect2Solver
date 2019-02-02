#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include "Connect2.h"
#include "RecursionHelper.h"
#include "Direction.h"
#include "MoveInput.h"


void solve(std::string fileName);
void play(std::string fileName);

int main()
{
	std::string fileName = "in.txt";

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
			std::cout << game.getDisplayStr() << "\n";
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
		std::cout << "Paths: " << m.gameInputArr.size() << "\n";

		//##################################################
		if (true && m.movesEvaluated > 10 * 1000000) {
			std::ofstream out("out.txt");
			out << m.idManager.getIdCountStr();
			out.close();
			return;
		}
		if (false && m.movesEvaluated > 40 * 1000000) {
			//std::ofstream out("out.txt");
			//out << "-----------------------------------------------------\n";
			std::mt19937_64 gen{ std::random_device()() };
			std::uniform_int_distribution<int> dis{ 0, (int)m.gameInputArr.size() };
			for (int i = 0; i < 100; i++) {
				int index = dis(gen);

				// Get GameInput from list
				auto iter = m.gameInputArr.begin();
				for (int j = 0; j < index; j++) {
					iter++;
				}

				m.game.reset();
				std::cout << m.game.getDisplayStr() << "\n";
				std::string str;
				std::cin >> str;

				auto iter2 = (*iter).miArr.begin();
				for (int j = 0; j < (*iter).miArr.size(); j++) {
					m.game.move(*iter2);
					std::cout << m.game.getDisplayStr() << "\n";
					std::cin >> str;

					iter2++;
					if (j == (*iter).miArr.size() - 1)
						std::cout << "End of game input.\n";
				}

				//out << m.game.getGameInputDisplayStr(*iter) << "\n---\n";
			}
			//out.close();
			return;
		}
		//##################################################
		
		int inputsSize = m.gameInputArr.size();
		if (inputsSize == 0) { // This means no more paths.
			return;
		}
		int count = 0;
		for (const GameInput& gi : m.gameInputArr) {
			count++;
			if (count > inputsSize)
				break;

			int numOfPaths = m.game.getNumOfPaths();
			std::vector<Direction> paths{ Direction::down, Direction::up, Direction::left, Direction::right };
			for (int i = 0; i < numOfPaths; i++) {
				for (int j = 0; j < paths.size(); j++) {
					// Setup board
					m.game.reset();
					for (const MoveInput& mi : gi.miArr) {
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
						bool isUnique = m.idManager.addIdIsUnique(id);
						if (!isUnique) // If game id already exists
							continue;

						// If we made it this far then move is valid and leads to a unique board state.
						GameInput newGi = gi;
						newGi.miArr.push_back(newMi);
						if (m.game.isSolved()) { // If we found a solution...
							m.solution = newGi;
							return;
						}

						m.gameInputArr.push_back(newGi);
					}
				}
			}
		}
		
		for (int i = 0; i < inputsSize; i++) {
			m.gameInputArr.pop_front();
		}
	}
}

void solve(std::string fileName)
{
	while (true) {
		Connect2 game(fileName);
		RecursionHelper m(game);
		solveRecursion(m);
		m.clearAllUnnecessaryData();
		std::cout << m.outputStr() << "\n";
		std::string i;
		std::cin >> i;
		std::cout << "---------------------------------------\n";
		if (i[0] == char('q'))
			return;
	}
}
