#pragma once
#include <string>
#include <memory>
#include "GameInput.h"
#include "Connect2.h"
#include "IdManager.h"
#include "MoveInputTree.h"
#include "SolvedInterface.h"
#include "SolveOptions.h"


class RecursionStruct
{
public:
	RecursionStruct(Connect2 game, SolveOptions so);
	void addSolution(GameInput gi);
	std::string getDisplayStr();
	
public:
	Connect2 game;
	Connect2 gameStart;
	bool solutionFound;
	std::list<GameInput> solutionList;
	std::unique_ptr<IdManager> idManager;
	GameInput gi;
	std::unique_ptr<MoveInputTree> mit;
	std::unique_ptr<SolvedInterface> si;
	bool solveEndPos;

	int movesEvaluated;
	int movesEvaluatedValid;
	int depth;
	int numBranches;
};
