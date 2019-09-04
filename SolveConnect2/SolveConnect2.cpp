#include <iostream>
#include <string>
#include <sstream>
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
	char charLoadGameInput = 'k';
	char charResetSolution = 'j';
	char charSolveEndState = 'y';

	std::string fileName = "level.txt";

	while (true) {
		Connect2 game(fileName);
		SolutionInputManager sim(game);
		Connect2 gameSave = game;
		SolutionInputManager simSave(game);

		while (true) {
			// Get user input
			game.getDisplayStr();
			std::cout << "Enter a command: ";
			std::string input;
			std::getline(std::cin, input);

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
			bool loadGameInput = false;
			bool resetSolution = false;
			bool solveEndState = false;
			bool help = false;
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
				else if (input[0] == charResetSolution)
					resetSolution = true;
				else if (input[0] == charSolveEndState)
					solveEndState = true;
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
			else if (input.size() > 3 && input[0] == charLoadGameInput && input[1] == ' ') {
				loadGameInput = true;
			}
			else if (input == "help") {
				help = true;
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
			}
			else if (displaySolution) {
				GameInput gi = sim.getSolution();
				std::cout << "Current moves: " << std::to_string(gi.miArr.size()) << "\n";
				std::cout << gi.getDisplayStr() << "\n";
				std::cout << "Save string:" << gi.getSaveString() << "\n";
			}
			else if (playSolution) {
				std::cout << "Playing current moves...\n";
				Connect2 gameTemp = sim.getGameStart();
				GameInput gi = sim.getSolution();
				for (auto iter = gi.miArr.begin(); iter != gi.miArr.end(); iter++) {
					gameTemp.getDisplayStr();
					std::cout << "\n";
					Sleep(600);
					gameTemp.move(*iter);
				}
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
				simSave = sim;
				std::cout << "Saved state.\n";
			}
			else if (loadGame) {
				game = gameSave;
				sim = simSave;
				std::cout << "Loaded state.\n";
			}
			else if (solveToCurrentPos) {
				std::cout << "Solving from saved state to current state...\n";
				Connect2Solver::solveCustom(gameSave, game.getIdStr());
			}
			else if (solveFromCurrentPos) {
				std::cout << "Solving from current state to saved state...\n";
				Connect2Solver::solveCustom(game, gameSave.getIdStr());
			}
			else if (loadGameInput) {
				std::string giStr = input.substr(2);
				GameInput gi(giStr);
				for (MoveInput mi : gi.miArr) {
					game.move(mi);
					sim.addMove(mi);
				}
				std::cout << "Loaded game input.\n";
			}
			else if (resetSolution) {
				sim = SolutionInputManager(game);
				std::cout << "Solution reset.\n";
			}
			else if (solveEndState) {
				std::cout << "Solving end state...\n";
				Connect2Solver::solveEndState(game);
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
				MoveInput mi(input[0] - '0', charToDirection(dirChar), singleStep);
				game.move(mi);
				sim.addMove(mi);
			}
			else if (help) {
				std::cout << "===================== List of commands =====================\n";
				std::cout << "<number>" << charLeft << " : Move a path left : ex) 1" << charLeft << "\n";
				std::cout << "<number>" << charRight << " : Move a path right : ex) 1" << charRight << "\n";
				std::cout << "<number>" << charUp << " : Move a path up : ex) 1" << charUp << "\n";
				std::cout << "<number>" << charDown << " : Move a path down : ex) 1" << charDown << "\n";
				std::cout << charReset << " : Reset the board : ex) " << charReset << "\n";
				std::cout << charReload << " : Reload the board from the file " << fileName << " : ex) " << charReload << "\n";
				std::cout << charSave << " : Save the current board state : ex) " << charSave << "\n";
				std::cout << charLoad << " : Load the saved board state : ex) " << charLoad << "\n";
				std::cout << charLoadGameInput << " <inputs> : Do all the typed moves : ex) " << charLoadGameInput << " 1" << charDown << " 1" << charRight << " 2" << charLeft << " 2" << charUp << " 1" << charRight << "\n";
				std::cout << charSingleStep << " : Put at the end of a move command to only move one step in that direction : ex) 1" << charDown << charSingleStep << "\n";
				std::cout << charSolve << " : Attempt to find a solution, given the current board state : ex) " << charSolve << "\n";
				std::cout << charSolveFromCurrent << " : Attempt to find a solution from the current board state to the saved board state : ex) " << charSolveFromCurrent << "\n";
				std::cout << charSolveToCurrent << " : Attempt to find a solution from the saved board state to the current board state : ex) " << charSolveToCurrent << "\n";
				std::cout << charDisplaySolution << " : Display the currently inputted moves : ex) " << charDisplaySolution << "\n";
				std::cout << charPlaySolution << " : Play the currently inputted moves : ex) " << charPlaySolution << "\n";
				std::cout << charResetSolution << " : Reset the currently inputted moves : ex) " << charResetSolution << "\n";
				std::cout << charSolveEndState << " : Attempt to find possible board states for solutions (does not find solutions themselves) : ex) " << charSolveEndState << "\n";
				std::cout << "============================================================\n";
			}

			std::cout << "\n";
		}
	}
	std::cout << "------------------------------------------------\n";

	return 0;
}
