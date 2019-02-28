#pragma once
#pragma comment(lib, "sqlite3x64.lib")
#include <string>
#include "sqlite3.h"


class Sqlite
{
public:
	Sqlite();
	~Sqlite();
	int openDb(std::string fileName);
	int closeDb();
	int execute(std::string stmt);

protected:
	int prepare(std::string stmt);
	int step();
	int finalize();

public:
	sqlite3* m_sql;
	sqlite3_stmt* m_stmt; // You can use this, or use your own, this is just here for convinience.

protected:
	sqlite3_stmt* m_stmtProt;
};

