#include "IdManagerSqlite.h"
#include <Windows.h>


IdManagerSqlite::IdManagerSqlite()
{
	DeleteFile(L"hash.db"); // Make sure database starts out fresh
	sql.openDb("hash.db"); // This should probably have some error checking, but I don't want to right now.
	sql.execute("PRAGMA synchronous = OFF;");
	sql.execute("create table HashTable(Hash text UNIQUE);");
}

IdManagerSqlite::~IdManagerSqlite()
{
	sql.finalize();
	sql.closeDb();
	DeleteFile(L"hash.db");
}

bool IdManagerSqlite::addIdIsUnique(std::string gameId)
{
	int result = sqlite3_bind_text(sql.m_stmt, 1, gameId.c_str(), gameId.size(), 0);
	if (result != 0) // bind_text should always return 0
		return false;
	result = sqlite3_step(sql.m_stmt);

	sqlite3_clear_bindings(sql.m_stmt);
	sqlite3_reset(sql.m_stmt);

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
	DeleteFile(L"hash.db");
}
