#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Connect2.h"
#include "MoveInput.h"


class RecursionHelper
{
public:
	RecursionHelper(Connect2 game);
	~RecursionHelper();
	std::string outputStr();

public:
	Connect2 game;
	std::list<std::list<MoveInput>> moveInputArrArr;
	std::list<MoveInput> solution;
	std::map<std::string, bool> gameStateId;
	int movesEvaluated;
	int movesEvaluatedValid;
	int movesEvaluatedDepth;
};