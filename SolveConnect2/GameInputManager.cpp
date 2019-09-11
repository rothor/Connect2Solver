#include "GameInputManager.h"
#include <Windows.h>


GameInputManager::GameInputManager() :
	m_dbFileName("input.sqlite"),
	m_max(0)
{
	DeleteFileA(m_dbFileName.c_str()); // delete the database file if it exists
	sqlite3_open_v2(m_dbFileName.c_str(), &m_conn, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0); // open the database
	std::string stmtStr = "PRAGMA synchronous = OFF"; // faster
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "PRAGMA JOURNAL_MODE = OFF"; // faster
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "CREATE TABLE Tbl(Id INTEGER PRIMARY KEY ASC, Input TEXT)"; // create table
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "BEGIN TRANSACTION"; // Begin the transaction. Everything that happens in this database will occur in one single transaction. The transaction ends when this object is destroyed.
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);

	stmtStr = "INSERT INTO Tbl (Input) VALUES (?)"; // prepare the insert statement
	sqlite3_prepare_v2(m_conn, stmtStr.c_str(), stmtStr.size(), &m_stmtInsert, 0);
	stmtStr = "SELECT max(Id) FROM Tbl"; // prepare the select max statement
	sqlite3_prepare_v2(m_conn, stmtStr.c_str(), stmtStr.size(), &m_stmtSelectMax, 0);
	stmtStr = "SELECT Input FROM Tbl WHERE Id <= ?"; // prepare the select less than statement
	sqlite3_prepare_v2(m_conn, stmtStr.c_str(), stmtStr.size(), &m_stmtSelectLess, 0);
	stmtStr = "DELETE FROM Tbl WHERE Id <= ?"; // prepare the delete statement
	sqlite3_prepare_v2(m_conn, stmtStr.c_str(), stmtStr.size(), &m_stmtDelete, 0);
}

GameInputManager::~GameInputManager()
{
	sqlite3_finalize(m_stmtInsert); // finalize the statements
	sqlite3_finalize(m_stmtSelectMax);
	sqlite3_finalize(m_stmtSelectLess);
	sqlite3_finalize(m_stmtDelete);
	std::string stmtStr = "END TRANSACTION"; // end the transaction
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	sqlite3_close(m_conn); // close the database
	DeleteFileA(m_dbFileName.c_str()); // delete the database file
}

void GameInputManager::getRows()
{
	sqlite3_reset(m_stmtSelectMax);
	sqlite3_step(m_stmtSelectMax);
	m_max = sqlite3_column_int(m_stmtSelectMax, 0);
	if (m_max != 0) {
		sqlite3_clear_bindings(m_stmtSelectLess);
		sqlite3_reset(m_stmtSelectLess);
		sqlite3_bind_int(m_stmtSelectLess, 1, m_max);
	}
}

bool GameInputManager::nextRow(GameInput& gi)
{
	if (m_max == 0) {
		gi = GameInput();
		m_max = -1;
		return true;
	}
	else if (m_max == -1) {
		return false;
	}

	int rc = sqlite3_step(m_stmtSelectLess);
	if (rc == SQLITE_ROW) {
		std::string giStr = (const char*)sqlite3_column_text(m_stmtSelectLess, 0);
		gi = GameInput(giStr);
		return true;
	}
	else {
		return false;
	}	
}

void GameInputManager::addGameInput(GameInput& gi)
{
	sqlite3_clear_bindings(m_stmtInsert);
	sqlite3_reset(m_stmtInsert);
	std::string giStr = gi.getSaveString();
	sqlite3_bind_text(m_stmtInsert, 1, giStr.c_str(), giStr.size(), 0);
	sqlite3_step(m_stmtInsert);
}

void GameInputManager::removePrevGameInputs()
{
	sqlite3_clear_bindings(m_stmtDelete);
	sqlite3_reset(m_stmtDelete);
	sqlite3_bind_int(m_stmtDelete, 1, m_max);
	sqlite3_step(m_stmtDelete);
}