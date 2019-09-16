#include "IdManagerSqlite.h"
#include <Windows.h>


IdManagerSqlite::IdManagerSqlite() :
	m_dbFileName("hash.sqlite")
{
	DeleteFileA(m_dbFileName.c_str()); // delete the database file if it exists
	sqlite3_open_v2(m_dbFileName.c_str(), &m_conn, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0); // open the database
	std::string stmtStr = "PRAGMA synchronous = OFF"; // faster
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "PRAGMA JOURNAL_MODE = OFF"; // faster
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "CREATE TABLE Tbl(Hash TEXT UNIQUE)"; // create table
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "BEGIN TRANSACTION"; // Begin the transaction. Everything that happens in this database will occur in one single transaction. The transaction ends when this object is destroyed.
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "INSERT INTO Tbl (Hash) VALUES (?)"; // prepare the insert statement
	sqlite3_prepare_v2(m_conn, stmtStr.c_str(), stmtStr.size(), &m_stmt, 0);
}

IdManagerSqlite::~IdManagerSqlite()
{
	sqlite3_finalize(m_stmt); // finalize the statement
	std::string stmtStr = "END TRANSACTION;"; // end the transaction
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	sqlite3_close(m_conn); // close the database
	DeleteFileA(m_dbFileName.c_str()); // delete the database file
}

bool IdManagerSqlite::addIdIsUnique(std::string gameId)
{
	sqlite3_clear_bindings(m_stmt);
	sqlite3_reset(m_stmt);
	sqlite3_bind_text(m_stmt, 1, gameId.c_str(), gameId.size(), 0);
	int result = sqlite3_step(m_stmt);

	if (result == SQLITE_DONE) // If it successfully inserted
		return true;
	else if (result == SQLITE_CONSTRAINT) // If query fails because of the 'unique' constraint, and thus is not unique
		return false;
	else // This should never happen. If it does, there's probably something wrong with the program.
		return false;
}