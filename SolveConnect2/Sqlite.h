#pragma once
#pragma comment(lib, "sqlite3x64.lib")
#include <string>
#include <memory>
#include "sqlite3.h"


class Sqlite
{
public:
	Sqlite();
	~Sqlite();
	int openDb(std::string fileName);
	int closeDb();
	int execute(std::string stmt);
	int prepare(std::string stmt);
	int preparev2(std::string stmt);
	int resetPreparedStmt();
	int step();
	int finalize();

protected:
	std::string strToUtf8(std::string str);

public:
	sqlite3* m_sql;
	sqlite3_stmt* m_stmt;
};

