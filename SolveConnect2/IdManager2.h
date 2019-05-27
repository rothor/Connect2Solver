#pragma once
#include "Sqlite.h"
#include <string>
#include <vector>


class IdManager2
{
public:
	IdManager2();

	bool addIdIsUnique(std::string gameId);
	void beginQuerying();
	void endQuerying();
	void clear();

protected:
	Sqlite m_sql;
	std::vector<sqlite3_stmt*> m_stmtArr;
	int m_tableCount;
};
