#pragma once
#include "sqlite3.h"
#include "IdManager.h"
#include <string>


class IdManagerSqlite : public IdManager
{
public:
	IdManagerSqlite();
	~IdManagerSqlite();

	bool addIdIsUnique(std::string gameId);

protected:
	sqlite3* m_conn;
	sqlite3_stmt* m_stmt;
	std::string m_dbFileName;
};
