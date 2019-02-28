#include "IdManager.h"
#include <Windows.h>

#include "Timer.h"
#include "HighResolutionTimer.h"


void deleteFile2(const wchar_t* file)
{
	DeleteFile(file);
	DWORD attrib = GetFileAttributes(file);
	while (attrib != INVALID_FILE_ATTRIBUTES) {
		attrib = GetFileAttributes(file);
		Sleep(1);
	}
}

IdManager::IdManager() :
	m_tableCount(1000)
{
	DeleteFile(L"hash.sqlite"); // Make sure database starts out fresh
	m_sql.openDb("hash.sqlite");
	m_sql.execute("PRAGMA synchronous = OFF;"); // faster
	m_sql.execute("PRAGMA JOURNAL_MODE = OFF;"); // faster
	m_sql.execute("CREATE TABLE HashTable(Hash TEXT UNIQUE);");
}

IdManager::~IdManager()
{
	
}

bool IdManager::addIdIsUnique(std::string gameId)
{
	HighResolutionTimer timer; // #####
	timer.reset(); // #####
	int rc = sqlite3_bind_text(m_sql.m_stmt, 1, gameId.c_str(), gameId.size(), 0);
	int result = sqlite3_step(m_sql.m_stmt);
	rc = sqlite3_clear_bindings(m_sql.m_stmt);
	rc = sqlite3_reset(m_sql.m_stmt);
	Timer::addTime(std::string("Hash"), timer.readMicro()); // #####

	if (result == SQLITE_DONE) // If it successfully inserted
		return true;
	else if (result == SQLITE_CONSTRAINT) // If query fails the 'unique' constraint, and thus is not unique
		return false;
	else // This should also probably have better error checking.
		return false;
}

void IdManager::beginQuerying()
{
	m_sql.execute("BEGIN TRANSACTION;");
	std::string stmt = "insert into HashTable (Hash) values (?);";
	int rc = sqlite3_prepare_v2(m_sql.m_sql, stmt.c_str(), stmt.size(), &m_sql.m_stmt, 0);
}

void IdManager::endQuerying()
{
	int rc = sqlite3_finalize(m_sql.m_stmt);
	//
	m_sql.execute("END TRANSACTION;");
}

void IdManager::clear()
{
	m_sql.execute("DELETE FROM HashTable;");
}
