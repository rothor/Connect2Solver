#include <iostream>
#include <string>
#include <Windows.h>
#include "Connect2.h"
#include "Direction.h"
#include "MoveInput.h"
#include "SolutionInputManager.h"
#include "Connect2Solver.h"


int main()
{
	std::string fileName = "level.txt";

	while (true) {
		Connect2 game(fileName);
		SolutionInputManager sim(game);
		GameInput startGim;

		while (true) {
			// Get user input
			std::cout << game.getDisplayStr() << "\n";
			std::cout << "Enter a command: ";
			std::string input;
			std::cin >> input;

			// Parse and get command from input
			bool reset = false;
			bool displaySolution = false;
			bool playSolution = false;
			bool invalid = false;
			bool move = false;
			bool undo = false;
			bool solve = false;
			bool reload = false;
			bool setStartGim = false;
			bool solveToCurrentPos = false;
			if (input.size() == 1) {
				if (input[0] == 't')
					reset = true;
				else if (input[0] == 'o')
					displaySolution = true;
				else if (input[0] == 'i')
					playSolution = true;
				else if (input[0] == 'n')
					undo = true;
				else if (input[0] == 's')
					solve = true;
				else if (input[0] == 'm')
					reload = true;
				else if (input[0] == 'g')
					setStartGim = true;
				else if (input[0] == 'c')
					solveToCurrentPos = true;
				else
					invalid = true;
			}
			else if (input.size() == 2 || input.size() == 3) {
				bool idIsValid = input[0] >= '1' && input[0] <= '9';
				bool directionIsValid = input[1] == 'u' || input[1] == 'd' ||
					input[1] == 'l' || input[1] == 'r';
				bool oneValid = input.size() == 2 || input[2] == 'o';
				if (idIsValid && directionIsValid && oneValid)
					move = true;
				else
					invalid = true;
			}
			else {
				invalid = true;
			}

			// Execute command
			if (invalid) {
				std::cout << "Invalid input.\n";
			}
			else if (reset) {
				game.reset();
				sim.reset();
				startGim.miArr.clear();
			}
			else if (undo) {
				game.undo();
			}
			else if (displaySolution) {
				GameInput gi = sim.getSolution();
				std::cout << "Moves: " << std::to_string(gi.miArr.size()) << "\n";
				std::cout << gi.getDisplayStr() << "\n";
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
			}
			else if (solve) {
				Connect2Solver::solve(fileName, startGim);
			}
			else if (reload) {
				std::cout << "\n";
				break;
			}
			else if (setStartGim) {
				startGim = sim.getSolution();
			}
			else if (solveToCurrentPos) {
				Connect2Solver::solveCustom(fileName, startGim, game.getIdStr());
			}
			else if (move) {
				bool singleStep = input.size() == 3;
				MoveInput mi(input[0] - '0', charToDirection(input[1]));
				if (!singleStep)
					game.move(mi);
				else
					game.moveOne(mi);
				sim.addMove(mi);
			}

			std::cout << "\n";
		}
	}
	std::cout << "------------------------------------------------\n";

	return 0;
}
