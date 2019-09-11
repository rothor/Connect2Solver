#pragma once
#include "sqlite3.h"
#include <string>


class IdManager
{
public:
	IdManager();
	~IdManager();

	bool addIdIsUnique(std::string gameId);

protected:
	sqlite3* m_conn;
	sqlite3_stmt* m_stmt;
	std::string m_dbFileName;
};
