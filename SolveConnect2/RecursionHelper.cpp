#include "RecursionHelper.h"


RecursionHelper::RecursionHelper(Connect2 game) :
	movesEvaluated(0),
	movesEvaluatedValid(0),
	movesEvaluatedDepth(1),
	game(game)
{
	gameInputArr.push_back(GameInput());
	m_gameStateId = std::vector<std::unique_ptr<std::map<std::string, bool>>>();
	for (int i = 0; i < m_numOfIdMaps; i++) {
		m_gameStateId.push_back(
			std::unique_ptr<std::map<std::string, bool>>(
				new std::map<std::string, bool>()
			)
		);
	}
	addGameIdIsUnique(game.getIdStr());
}

RecursionHelper::~RecursionHelper()
{
	
}

std::string RecursionHelper::outputStr()
{
	std::string retStr = "Moves evaluated: " + std::to_string(movesEvaluated) + "\n";
	
	if (solution.miArr.size() == 0) {
		retStr += "Solution not found.\n";
		return retStr;
	}

	retStr += "Solution (" + std::to_string(solution.miArr.size()) + " moves):";
	retStr += game.getGameInputDisplayStr(solution);
	retStr += "\n";

	return retStr;
}


unsigned int DJBHash(const char* str, unsigned int length)
{
	unsigned int hash = 5381;
	unsigned int i = 0;
	for (i = 0; i < length; ++str, ++i) {
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;
}

bool RecursionHelper::addGameIdIsUnique(std::string gameId)
{
	unsigned int hashValue = DJBHash(&gameId[0], gameId.length());
	hashValue %= m_numOfIdMaps;
	auto ret = m_gameStateId[hashValue]->insert(std::pair<std::string, bool>(gameId, false));
	return ret.second;
}

void RecursionHelper::clearAllUnnecessaryData()
{
	gameInputArr.clear();
	m_gameStateId.clear();
}