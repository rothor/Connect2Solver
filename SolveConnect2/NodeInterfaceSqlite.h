#pragma once
#include "NodeInterface.h"
#include "sqlite3.h"
#include <list>


class NodeInterfaceSqlite : public NodeInterface
{
public:
	NodeInterfaceSqlite(const MoveInput& mi, int id, sqlite3_stmt* selectChildren, sqlite3_stmt* addChild, sqlite3_stmt* deletez);
	~NodeInterfaceSqlite();
	bool getNextChild(std::shared_ptr<NodeInterface>& node);
	void addChild(MoveInput mi);
	void deleteCurrentChild();
	GameInput getGameInput();

protected:
	int getId();

	MoveInput m_mi;
	int m_id;
	std::list<std::shared_ptr<NodeInterfaceSqlite>> m_children;
	std::list<std::shared_ptr<NodeInterfaceSqlite>>::iterator m_it;
	bool m_itInitialized;
	bool m_deleted;
	sqlite3_stmt* m_stmtSelectChildren;
	sqlite3_stmt* m_stmtAddChild;
	sqlite3_stmt* m_stmtDelete;
};