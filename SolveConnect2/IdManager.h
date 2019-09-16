#pragma once
#include <string>


class IdManager
{
public:
	virtual ~IdManager() = 0;
	virtual bool addIdIsUnique(std::string gameId) = 0;
};