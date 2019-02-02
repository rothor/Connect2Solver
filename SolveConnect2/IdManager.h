#pragma once
#include <map>
#include <vector>
#include <memory>
#include <string>


class IdManager
{
public:
	IdManager();
	bool addIdIsUnique(std::string gameId);
	void clear();
	// for id counting
	std::string getIdCountStr();

protected:
	void addIdCount(std::string gameId, unsigned int hash);
	
protected:
	std::vector<std::unique_ptr<std::map<std::string, bool>>> m_idMapArr;
	const int m_numOfIdMaps;
	// for id counting
	bool m_doIdCounting;
	std::vector<std::unique_ptr<std::map<std::string, int>>> m_idCountMapArr;
};
