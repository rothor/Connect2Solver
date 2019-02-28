#pragma once
#include <string>
#include "GameInput.h"
#include "Connect2.h"
#include "IdManager.h"
#include "GameInputManager.h"


class RecursionStruct
{
public:
	RecursionStruct(Connect2 game);
	std::string getDisplayStr();
	
public:
	Connect2 game;
	GameInput gi;
	int movesEvaluated;
	int movesEvaluatedValid;
	bool solutionFound;
	GameInput solution;
	int depth;
	int numBranches;
	IdManager* idManager;
	GameInputManager* gim;
};
