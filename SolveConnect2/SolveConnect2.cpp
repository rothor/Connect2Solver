#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <random>
#include <Windows.h>
#include "Connect2.h"
#include "RecursionHelper.h"
#include "Direction.h"
#include "MoveInput.h"
#include "SolutionInputManager.h"


void solve(std::string fileName);
void play(std::string fileName);

void test()
{
	
}

int main()
{
	test();
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
	SolutionInputManager sim(game);

	while (true) {
		game.reset();
		sim.reset();
		while (true) {
			std::cout << game.getDisplayStr() << "\n";
			std::cout << "Enter a command: ";
			std::string input;
			std::cin >> input;

			// check input
			bool quit = false;
			bool reset = false;
			bool displaySolution = false;
			bool playSolution = false;
			bool invalid = false;
			bool move = false;
			if (input.size() == 1) {
				if (input[0] == 'q')
					quit = true;
				else if (input[0] == 'e')
					reset = true;
				else if (input[0] == 'o')
					displaySolution = true;
				else if (input[0] == 'i')
					playSolution = true;
				else
					invalid = true;
			}
			else if (input.size() >= 2) {
				invalid = true;
				if (input[0] >= '1' && input[0] <= '9') {
					if (input[1] == 'u' || input[1] == 'd' ||
						input[1] == 'l' || input[1] == 'r')
						invalid = false;
				}
			}

			if (invalid) {
				std::cout << "Invalid input.\n";
				std::cout << "\n";
				continue;
			}
			else if (reset)
				break;
			else if (quit)
				return;
			else if (displaySolution) {
				GameInput gi = sim.getSolution();
				std::cout << "Moves: " << std::to_string(gi.miArr.size()) << "\n";
				std::cout << gi.getDisplayStr() << "\n";
				std::cout << "\n";
				continue;
			}
			else if (playSolution) {
				game.reset();
				GameInput gi = sim.getSolution();
				for (auto iter = gi.miArr.begin(); iter != gi.miArr.end(); iter++) {
					std::cout << game.getDisplayStr() << "\n";
					std::cout << "\n";
					Sleep(600);
					game.move(*iter);
				}
				std::cout << game.getDisplayStr() << "\n";
				std::cout << "\n";
				continue;
			}

			std::string pathIndexStr = input.substr(0, 1);
			MoveInput mi(input[0] - '0', charToDirection(input[1]));

			game.move(mi);
			sim.addMove(mi);
			std::cout << "\n";
		}
	}
	std::cout << "------------------------------------------------\n";
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

void solveRecursion(RecursionHelper& m)
{
	while (true) {
		m.movesEvaluatedDepth++; // for display
		std::cout << "Depth: " << m.movesEvaluatedDepth << "\n";
		std::cout << "Paths: " << formatIntWithCommas(m.gameInputArr.size()) << "\n";
		std::cout << "Evaluated: " << formatIntWithCommas(m.movesEvaluated) << "\n";

		//##################################################
		if (false && m.movesEvaluated > 50 * 1000000) {
			return;
		}
		if (false && m.movesEvaluated > 10 * 1000000) {
			std::ofstream out("out.txt");
			//out << m.idManager.getIdCountStr();
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

		for (int k = 0; k < inputsSize; k++) {
			GameInput* gi = &m.gameInputArr.front();
			int numOfPaths = m.game.getNumOfPaths();
			std::vector<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };
			for (int i = 0; i < numOfPaths; i++) {
				for (int j = 0; j < directions.size(); j++) {
					// Setup board
					m.game.reset();
					for (const MoveInput& mi : gi->miArr) {
						m.game.move(mi);
					}

					// Evaluate new move
					MoveInput newMi = MoveInput(m.game.getPathDisplayId(i), directions[j]);
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
						GameInput newGi = *gi;
						newGi.miArr.push_back(newMi);
						if (m.game.isSolved()) { // If we found a solution...
							m.solution = newGi;
							return;
						}

						m.gameInputArr.push_back(newGi);
					}
				}
			}

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
