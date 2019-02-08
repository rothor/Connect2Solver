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
#include "GameInputRecursionNode.h"


void solve(std::string fileName);
void play(std::string fileName);

void test()
{
	std::cout << "---------- Begin Test ----------\n";
	std::list<int> myList;
	myList.push_back(0);
	myList.push_back(1);
	myList.push_back(2);
	myList.push_back(3);
	myList.push_back(4);
	int i = 0;
	for (auto iter = myList.begin(); iter != myList.end(); ) {
		if (i == 2) {
			iter = myList.erase(iter);
			i++;
			continue;
		}
		std::cout << *iter << ", ";
		i++;
		iter++;
	}
	std::cout << "\n---------- End Test ----------\n";
}

int main()
{
	//test();
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

	return 0;
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
			bool undo = false;
			if (input.size() == 1) {
				if (input[0] == 'q')
					quit = true;
				else if (input[0] == 'e')
					reset = true;
				else if (input[0] == 'o')
					displaySolution = true;
				else if (input[0] == 'i')
					playSolution = true;
				else if (input[0] == 'n')
					undo = true;
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
			else if (undo) {
				game.undo();
				std::cout << "\n";
				continue;
			}
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

void solveRecursion(GameInputRecursionNodeInfo& gii)
{
	MoveInput mi;
	GameInputRecursionNode node(mi);
	while (node.recurse(gii)) {
		gii.depth++;

		if (false && gii.depth > 10) {
			break;
		}

		std::cout << "Depth: " << gii.depth << "\n";
		std::cout << "Paths: " << formatIntWithCommas(gii.numBranches) << "\n";
		std::cout << "Evaluated: " << formatIntWithCommas(gii.movesEvaluated) << "\n";

		if (gii.game.isSolved())
			break;

		gii.numBranches = 0;
	}
	gii.clearUnnecessaryData();
}

void solve(std::string fileName)
{
	while (true) {
		Connect2 game(fileName);
		GameInputRecursionNodeInfo gii(game);
		solveRecursion(gii);
		std::cout << gii.getDisplayStr() << "\n";
		std::string i;
		std::cin >> i;
		std::cout << "---------------------------------------------------\n";
		if (i[0] == char('q'))
			return;
	}
}
