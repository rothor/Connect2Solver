#pragma once
#include "IdManager.h"
#include <string>
#include <map>


class IdManagerMemory : public IdManager
{
public:
	IdManagerMemory();
	~IdManagerMemory();

	bool addIdIsUnique(std::string gameId);

protected:
	std::map<std::string, bool> m_map;
};