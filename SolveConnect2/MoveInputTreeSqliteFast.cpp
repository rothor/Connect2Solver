#include "MoveInputTreeSqliteFast.h"
#include "NodeInterfaceSqliteFast.h"
#include <Windows.h>
#include <string>


MoveInputTreeSqliteFast::MoveInputTreeSqliteFast() :
	m_fileName("input.sqlite")
{
	DeleteFileA(m_fileName.c_str()); // delete the database file if it exists
	sqlite3_open_v2(m_fileName.c_str(), &m_conn, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0); // open the database
	std::string stmtStr = "PRAGMA synchronous = OFF"; // faster
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "PRAGMA JOURNAL_MODE = OFF"; // faster
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "CREATE TABLE Tbl(Id INTEGER PRIMARY KEY ASC, GameInput TEXT)"; // create table
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	stmtStr = "BEGIN TRANSACTION"; // Begin the transaction. Everything that happens in this database will occur in one single transaction. The transaction ends when this object is destroyed.
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);

	stmtStr = "SELECT GameInput FROM Tbl WHERE Id <= ?";
	sqlite3_prepare_v2(m_conn, stmtStr.c_str(), stmtStr.size(), &m_stmtSelectChildren, 0);
	stmtStr = "INSERT INTO Tbl (GameInput) VALUES (?)";
	sqlite3_prepare_v2(m_conn, stmtStr.c_str(), stmtStr.size(), &m_stmtAddChild, 0);
	stmtStr = "DELETE FROM Tbl WHERE Id <= ?";
	sqlite3_prepare_v2(m_conn, stmtStr.c_str(), stmtStr.size(), &m_stmtDelete, 0);
	stmtStr = "SELECT MAX(Id) FROM Tbl";
	sqlite3_prepare_v2(m_conn, stmtStr.c_str(), stmtStr.size(), &m_stmtGetMax, 0);
}

MoveInputTreeSqliteFast::~MoveInputTreeSqliteFast()
{
	sqlite3_finalize(m_stmtSelectChildren); // finalize the statements
	sqlite3_finalize(m_stmtAddChild);
	sqlite3_finalize(m_stmtDelete);
	sqlite3_finalize(m_stmtGetMax);
	std::string stmtStr = "END TRANSACTION"; // end the transaction
	sqlite3_exec(m_conn, stmtStr.c_str(), 0, 0, 0);
	sqlite3_close(m_conn); // close the database
	DeleteFileA(m_fileName.c_str()); // delete the database file
}

std::shared_ptr<NodeInterface> MoveInputTreeSqliteFast::getStartNode()
{
	return std::shared_ptr<NodeInterfaceSqliteFast>(
		new NodeInterfaceSqliteFast(GameInput(), true, m_stmtSelectChildren, m_stmtAddChild, m_stmtDelete, m_stmtGetMax)
	);
}