#pragma once
#include "Sqlite.h"
#include <string>


class IdManager
{
public:
	IdManager(int anInt = 1);
	~IdManager();

	bool addIdIsUnique(std::string gameId);
	void beginQuerying();
	void endQuerying();

protected:
	Sqlite m_sql;
	std::string m_fileName;
	static int instanceCount;
};
