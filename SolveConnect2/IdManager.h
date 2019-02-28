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
	void clear();

protected:
	Sqlite m_sql;
	std::vector<sqlite3_stmt*> m_stmtArr;
	int m_tableCount;
};
