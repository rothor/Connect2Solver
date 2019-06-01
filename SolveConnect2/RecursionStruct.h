#pragma once
#include <string>
#include "GameInput.h"
#include "Connect2.h"
#include "IdManager.h"
//#include "IdManager2.h"
#include "GameInputManager.h"


class RecursionStruct
{
public:
	RecursionStruct(Connect2 game);
	void addSolution(GameInput gi);
	std::string getDisplayStr();
	
public:
	Connect2 game;
	Connect2 gameStart;
	int movesEvaluated;
	int movesEvaluatedValid;
	bool solutionFound;
	std::list<GameInput> solutionList;
	int depth;
	int numBranches;
	IdManager idManager;
	GameInputManager gim;

protected:
	int instance;
	static int instanceCount;
};
