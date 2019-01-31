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
	std::list<Connect2> gameArr;
	std::list<std::list<MoveInput>> solution;
	std::list<MoveInput> move;
	std::map<std::string, int> gameId;
	int movesEvaluated;
	int movesEvaluatedValid;
	int movesEvaluatedDepth;
	int movesEvaluatedMaxDepth;
};