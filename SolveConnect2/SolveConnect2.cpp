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
	char charLeft = 's';
	char charRight = 'f';
	char charDown = 'd';
	char charUp = 'e';
	char charSingleStep = 'r';
	char charSolve = 'z';
	char charSolveToCurrent = 'c';
	char charSolveFromCurrent = 'v';
	char charReset = 't';
	char charReload = 'm';
	char charSave = 'g';
	char charLoad = 'h';
	char charDisplaySolution = 'i';
	char charPlaySolution = 'p';

	std::string fileName = "level.txt";

	while (true) {
		Connect2 game(fileName);
		SolutionInputManager sim(game);
		Connect2 gameSave = game;
		bool simValid = true;

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
			bool saveGame = false;
			bool solveToCurrentPos = false;
			bool solveFromCurrentPos = false;
			bool loadGame = false;
			if (input.size() == 1) {
				if (input[0] == charReset)
					reset = true;
				else if (input[0] == charDisplaySolution)
					displaySolution = true;
				else if (input[0] == charPlaySolution)
					playSolution = true;
				else if (input[0] == charSolve)
					solve = true;
				else if (input[0] == charReload)
					reload = true;
				else if (input[0] == charSave)
					saveGame = true;
				else if (input[0] == charSolveToCurrent)
					solveToCurrentPos = true;
				else if (input[0] == charSolveFromCurrent)
					solveFromCurrentPos = true;
				else if (input[0] == charLoad)
					loadGame = true;
				else
					invalid = true;
			}
			else if (input.size() == 2 || input.size() == 3) {
				bool idIsValid = input[0] >= '1' && input[0] <= '9';
				bool directionIsValid = input[1] == charLeft || input[1] == charRight ||
					input[1] == charDown || input[1] == charUp;
				bool oneValid = input.size() == 2 || input[2] == charSingleStep;
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
				std::cout << "Reset board.\n";
				game.reset();
				sim = SolutionInputManager(game);
				simValid = true;
			}
			else if (displaySolution) {
				if (simValid) {
					GameInput gi = sim.getSolution();
					std::cout << "Current moves: " << std::to_string(gi.miArr.size()) << "\n";
					std::cout << gi.getDisplayStr() << "\n";
				}
				else
					std::cout << "No longer valid. Reset to make valid again.\n";
			}
			else if (playSolution) {
				if (simValid) {
					std::cout << "Playing current moves...\n";
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
				else
					std::cout << "No longer valid. Reset to make valid again.\n";
			}
			else if (solve) {
				std::cout << "Solving...\n";
				Connect2Solver::solve(game);
			}
			else if (reload) {
				std::cout << "Reloaded file.\n\n";
				break;
			}
			else if (saveGame) {
				gameSave = game;
				std::cout << "Saved state.\n";
			}
			else if (solveToCurrentPos) {
				std::cout << "Solving from saved state to current state...\n";
				Connect2Solver::solveCustom(gameSave, game.getIdStr());
			}
			else if (solveFromCurrentPos) {
				std::cout << "Solving from current state to saved state...\n";
				Connect2Solver::solveCustom(game, gameSave.getIdStr());
			}
			else if (loadGame) {
				game = gameSave;
				sim = SolutionInputManager(game);
				std::cout << "Loaded state.\n";
			}
			else if (move) {
				bool singleStep = input.size() == 3 && input[2] == charSingleStep;
				char dirChar = 'l';
				if (input[1] == charUp)
					dirChar = 'u';
				else if (input[1] == charDown)
					dirChar = 'd';
				else if (input[1] == charRight)
					dirChar = 'r';
				MoveInput mi(input[0] - '0', charToDirection(dirChar));
				if (!singleStep) {
					game.move(mi);
					sim.addMove(mi);
				}
				else {
					game.moveOne(mi);
					simValid = false;
				}
			}

			std::cout << "\n";
		}
	}
	std::cout << "------------------------------------------------\n";

	return 0;
}
