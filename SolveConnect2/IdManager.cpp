#include "IdManager.h"
#include <Windows.h>


int IdManager::instanceCount = 1;

IdManager::IdManager(int anInt)
{
	m_fileName = std::string("hash" + std::to_string(instanceCount++) + ".sqlite");
	DeleteFileA(m_fileName.c_str());
	int rc = sqlite3_open(m_fileName.c_str(), &(m_sql.m_sql));
	std::string stmt = "PRAGMA synchronous = OFF;"; // faster
	rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
	stmt = "PRAGMA JOURNAL_MODE = OFF;"; // faster
	rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
	stmt = "CREATE TABLE HashTable(Hash TEXT UNIQUE);";
	rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
}

IdManager::~IdManager()
{
	int rc = sqlite3_close(m_sql.m_sql);
	DeleteFileA(m_fileName.c_str());
}

bool IdManager::addIdIsUnique(std::string gameId)
{
	int rc = sqlite3_bind_text(m_sql.m_stmt, 1, gameId.c_str(), gameId.size(), 0);
	int result = sqlite3_step(m_sql.m_stmt);
	rc = sqlite3_clear_bindings(m_sql.m_stmt);
	rc = sqlite3_reset(m_sql.m_stmt);

	if (result == SQLITE_DONE) // If it successfully inserted
		return true;
	else if (result == SQLITE_CONSTRAINT) // If query fails the 'unique' constraint, and thus is not unique
		return false;
	else // This should probably have better error checking.
		return false;
}

void IdManager::beginQuerying()
{
	std::string stmt = "BEGIN TRANSACTION;";
	int rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
	stmt = "INSERT INTO HashTable (Hash) VALUES (?);";
	rc = sqlite3_prepare_v2(m_sql.m_sql, stmt.c_str(), stmt.size(), &m_sql.m_stmt, 0);
}

void IdManager::endQuerying()
{
	int rc = sqlite3_finalize(m_sql.m_stmt);
	std::string stmt = "END TRANSACTION;";
	rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
}
