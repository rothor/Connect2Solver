#pragma once
#include <string>
#include <list>
#include "Connect2.h"
#include "MoveInput.h"
#include "GameInput.h"
#include "IdManager.h"


class RecursionHelper
{
public:
	RecursionHelper(Connect2 game);
	std::string outputStr();
	void clearAllUnnecessaryData();

public:
	Connect2 game;
	std::list<GameInput> gameInputArr;
	GameInput solution;
	int movesEvaluated;
	int movesEvaluatedValid;
	int movesEvaluatedDepth;
	IdManager idManager;
};