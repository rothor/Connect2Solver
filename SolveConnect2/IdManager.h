#pragma once
#include "Sqlite.h"
#include <string>
#include <vector>


class IdManager
{
public:
	IdManager();
	~IdManager();

	bool addIdIsUnique(std::string gameId);
	void beginQuerying();
	void endQuerying();

protected:
	Sqlite m_sql;
};
