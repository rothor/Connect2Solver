#pragma once
#include "NodeInterface.h"
#include "sqlite3.h"


class NodeInterfaceSqlite : public NodeInterface
{
public:
	NodeInterfaceSqlite(MoveInput mi, int id, sqlite3_stmt* selectChildren, sqlite3_stmt* addChild, sqlite3_stmt* deletez);
	~NodeInterfaceSqlite();
	std::list<std::shared_ptr<NodeInterface>> getChildren();
	void addChild(MoveInput mi);
	void deleteThisNode();
	MoveInput getMoveInput();

protected:
	MoveInput m_mi;
	int m_id;
	sqlite3_stmt* m_stmtSelectChildren;
	sqlite3_stmt* m_stmtAddChild;
	sqlite3_stmt* m_stmtDelete;
};