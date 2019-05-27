#include "IdManager2.h"
#include <Windows.h>


IdManager2::IdManager2() :
	m_tableCount(10000)
{
	DeleteFile(L"hash.sqlite"); // Make sure database starts out fresh
	int rc = sqlite3_open("hash.sqlite", &(m_sql.m_sql));
	std::string stmt = "PRAGMA synchronous = OFF;"; // faster
	rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
	stmt = "PRAGMA JOURNAL_MODE = OFF;"; // faster
	rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);

	m_stmtArr.resize(m_tableCount);
	for (int i = 0; i < m_tableCount; i++) {
		stmt = "CREATE TABLE HashTable" + std::to_string(i) + "(Hash TEXT UNIQUE);";
		rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
	}
}

unsigned int DJBHash(const char* str, unsigned int length)
{
	unsigned int hash = 5381;
	unsigned int i = 0;
	for (i = 0; i < length; ++str, ++i) {
		hash = ((hash << 5) + hash) + (*str);
	}
	return hash;
}

bool IdManager2::addIdIsUnique(std::string gameId)
{
	unsigned int hashValue = DJBHash(gameId.c_str(), gameId.size()) % m_tableCount;

	int rc = sqlite3_bind_text(m_stmtArr[hashValue], 1, gameId.c_str(), gameId.size(), 0);
	int result = sqlite3_step(m_stmtArr[hashValue]);
	rc = sqlite3_clear_bindings(m_stmtArr[hashValue]);
	rc = sqlite3_reset(m_stmtArr[hashValue]);

	if (result == SQLITE_DONE) // If it successfully inserted
		return true;
	else if (result == SQLITE_CONSTRAINT) // If query fails the 'unique' constraint, and thus is not unique
		return false;
	else // This should probably have better error checking.
		return false;
}

void IdManager2::beginQuerying()
{
	std::string stmt = "BEGIN TRANSACTION;";
	int rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
	
	for (int i = 0; i < m_tableCount; i++) {
		std::string stmt = "INSERT INTO HashTable" + std::to_string(i) + " (Hash) VALUES (?);";
		rc = sqlite3_prepare_v2(m_sql.m_sql, stmt.c_str(), stmt.size(), &(m_stmtArr[i]), 0);
	}
}

void IdManager2::endQuerying()
{
	int rc;
	for (int i = 0; i < m_tableCount; i++) {
		rc = sqlite3_finalize(m_stmtArr[i]);
	}

	std::string stmt = "END TRANSACTION;";
	rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
}

void IdManager2::clear()
{
	std::string stmt;
	int rc;
	for (int i = 0; i < m_tableCount; i++) {
		stmt = "DELETE FROM HashTable" + std::to_string(i) + ";";
		rc = sqlite3_exec(m_sql.m_sql, stmt.c_str(), 0, 0, 0);
	}
}
