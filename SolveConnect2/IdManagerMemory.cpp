#include "IdManagerMemory.h"


IdManagerMemory::IdManagerMemory()
{

}

IdManagerMemory::~IdManagerMemory()
{

}

bool IdManagerMemory::addIdIsUnique(std::string gameId)
{
	auto ret = m_map.insert(std::pair<std::string, bool>(gameId, false));
	return ret.second;
}