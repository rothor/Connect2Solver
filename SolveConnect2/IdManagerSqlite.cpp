#include "IdManagerSqlite.h"
#include <Windows.h>


void deleteFile2(const wchar_t* file)
{
	DeleteFile(file);
	DWORD attrib = GetFileAttributes(file);
	while (attrib != INVALID_FILE_ATTRIBUTES) {
		attrib = GetFileAttributes(file);
		Sleep(1);
	}
}

IdManagerSqlite::IdManagerSqlite()
{
	DeleteFile(L"hash.sqlite"); // Make sure database starts out fresh
	sql.openDb("hash.sqlite"); // This should probably have some error checking, but I don't want to right now.
	sql.execute("PRAGMA synchronous = OFF;");
	sql.execute("PRAGMA JOURNAL_MODE = OFF;");
	//sql.execute("PRAGMA LOCKING_MODE = EXCLUSIVE;");
	sql.execute("create table HashTable(Hash text UNIQUE);");
}

IdManagerSqlite::~IdManagerSqlite()
{
	sql.finalize();
	sql.closeDb();
	DeleteFile(L"hash.sqlite");
}

#include "Timer.h"
#include "HighResolutionTimer.h"
bool IdManagerSqlite::addIdIsUnique(std::string gameId)
{
	// --start sql
	HighResolutionTimer timer; // #####
	timer.reset(); // #####
	int result = sqlite3_bind_text(sql.m_stmt, 1, gameId.c_str(), gameId.size(), 0);
	if (result != 0) // bind_text should always return 0
		return false;
	result = sqlite3_step(sql.m_stmt);
	sqlite3_clear_bindings(sql.m_stmt);
	sqlite3_reset(sql.m_stmt);
	Timer::addTime(std::string("Hash"), timer.readMicro()); // #####
	// --end sql

	if (result == SQLITE_DONE) // If it successfully inserted
		return true;
	else if (result == SQLITE_CONSTRAINT) // If query fails the 'unique' constraint, and thus is not unique
		return false;
	else // This should also probably have better error checking.
		return false;
}

void IdManagerSqlite::beginQuerying()
{
	sql.execute("BEGIN TRANSACTION;");
	sql.preparev2("insert into HashTable (Hash) values (?);");
}

void IdManagerSqlite::endQuerying()
{
	sql.finalize();
	sql.execute("END TRANSACTION;");
}

void IdManagerSqlite::clear()
{
	sql.finalize();
	sql.closeDb();
	DeleteFile(L"hash.sqlite");
}
