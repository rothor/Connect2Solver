#include "IdManager.h"


IdManager::IdManager() :
	m_numOfIdMaps(1000000),
	m_doIdCounting(true) // set this to false to avoid id counting
{
	for (int i = 0; i < m_numOfIdMaps; i++) {
		m_idMapArr.push_back(
			std::unique_ptr<std::map<std::string, bool>>(
				new std::map<std::string, bool>()
			)
		);
	}

	if (m_doIdCounting) {
		for (int i = 0; i < m_numOfIdMaps; i++) {
			m_idCountMapArr.push_back(
				std::unique_ptr<std::map<std::string, int>>(
					new std::map<std::string, int>()
					)
			);
		}
	}
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

bool IdManager::addIdIsUnique(std::string gameId)
{
	unsigned int hashValue = DJBHash(&gameId[0], gameId.length());
	hashValue %= m_numOfIdMaps;
	auto ret = m_idMapArr[hashValue]->insert(std::pair<std::string, int>(gameId, false));

	if (m_doIdCounting) {
		addIdCount(gameId, hashValue);
	}

	return ret.second;
}

void IdManager::addIdCount(std::string gameId, unsigned int hash)
{
	auto ret = m_idCountMapArr[hash]->insert(std::pair<std::string, int>(gameId, 1));
	if (!ret.second)
		ret.first->second++;
}

void IdManager::clear()
{
	m_idMapArr.clear();
}

std::string IdManager::getIdCountStr()
{
	std::map<int, int> countMap;
	for (int i = 0; i < m_numOfIdMaps; i++) {
		for (auto pair : *m_idCountMapArr[i]) {
			auto iter = countMap.find(pair.second);
			if (iter == countMap.end())
				countMap.insert(std::pair<int, int>(pair.second, 1));
			else
				iter->second++;
		}
	}

	int total = 0;
	for (auto pair : countMap) {
		total += pair.second;
	}
	std::map<int, double> countPercentMap;
	for (auto pair : countMap) {
		countPercentMap.insert(std::pair<int, double>(pair.first, (double)pair.second / total));
	}
	
	std::string ret;
	double totalPercent = 0;
	for (auto pair : countPercentMap) {
		ret += std::to_string(pair.first) + "\t" + std::to_string(pair.second) + "\n";
		totalPercent += pair.second;
	}
	ret += "Total: " + std::to_string(totalPercent);
	return ret;
}
