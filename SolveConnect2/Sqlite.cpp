#include "Sqlite.h"
#include <Windows.h>


Sqlite::Sqlite()
{
	
}

Sqlite::~Sqlite()
{
	closeDb();
}

int Sqlite::openDb(std::string fileName)
{
	return sqlite3_open(fileName.c_str(), &m_sql);
}

int Sqlite::closeDb()
{
	if (m_sql == 0)
		return 0;
	int rc = sqlite3_close(m_sql);
	return rc;
}

int Sqlite::execute(std::string stmt)
{
	int rc = prepare(stmt);
	rc = step();
	rc = finalize();
	return rc;
}

int Sqlite::prepare(std::string stmt)
{
	return sqlite3_prepare(m_sql, stmt.c_str(), stmt.size(), &m_stmtProt, 0);
}

int Sqlite::step()
{
	return sqlite3_step(m_stmtProt);
}

int Sqlite::finalize()
{
	if (m_stmtProt == 0)
		return 3;
	int result = sqlite3_finalize(m_stmtProt);
	if (result == 0)
		m_stmtProt = 0;
	return result;
}
