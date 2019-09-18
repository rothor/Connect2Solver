#pragma once
#include "MoveInputTree.h"
#include "sqlite3.h"
#include <memory>
#include <string>


class MoveInputTreeSqlite : public MoveInputTree
{
public:
	MoveInputTreeSqlite();
	~MoveInputTreeSqlite();
	std::shared_ptr<NodeInterface> getStartNode();

protected:
	std::string m_fileName;
	sqlite3* m_conn;
	sqlite3_stmt* m_stmtSelectChildren;
	sqlite3_stmt* m_stmtAddChild;
	sqlite3_stmt* m_stmtDelete;
};