#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Connect2.h"
#include "MoveInput.h"
#include "GameInput.h"


class RecursionHelper
{
public:
	RecursionHelper(Connect2 game);
	~RecursionHelper();
	std::string outputStr();
	bool addGameIdIsUnique(std::string gameId);
	void clearAllUnnecessaryData();

public:
	Connect2 game;
	std::list<GameInput> gameInputArr;
	GameInput solution;
	int movesEvaluated;
	int movesEvaluatedValid;
	int movesEvaluatedDepth;

private:
	std::vector<std::unique_ptr<std::map<std::string, bool>>> m_gameStateId;
	const int m_numOfIdMaps{ 100000 };
};