#pragma once
#include <string>
#include "GameInput.h"
#include "Connect2.h"
//#include "IdManager.h"
#include "IdManagerSqlite.h"


class GameInputRecursionNodeInfo
{
public:
	GameInputRecursionNodeInfo(Connect2 game);
	void clearUnnecessaryData();
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
	IdManagerSqlite idManager;
};
