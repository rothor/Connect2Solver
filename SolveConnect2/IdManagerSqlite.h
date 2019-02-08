#pragma once
#include "Sqlite.h"
#include <string>


class IdManagerSqlite
{
public:
	IdManagerSqlite();
	~IdManagerSqlite();

	bool addIdIsUnique(std::string gameId);
	void beginQuerying();
	void endQuerying();
	void clear();

protected:
	Sqlite sql;
};
