#include "NodeInterfaceSqlite.h"


NodeInterfaceSqlite::NodeInterfaceSqlite(MoveInput mi, int id, sqlite3_stmt* selectChildren, sqlite3_stmt* addChild, sqlite3_stmt* deletez) :
	m_mi(mi),
	m_id(id),
	m_stmtSelectChildren(selectChildren),
	m_stmtAddChild(addChild),
	m_stmtDelete(deletez)
{

}

NodeInterfaceSqlite::~NodeInterfaceSqlite()
{

}

std::list<std::shared_ptr<NodeInterface>> NodeInterfaceSqlite::getChildren()
{
	std::list<std::shared_ptr<NodeInterface>> ret;
	sqlite3_clear_bindings(m_stmtSelectChildren);
	sqlite3_reset(m_stmtSelectChildren);
	sqlite3_bind_int(m_stmtSelectChildren, 1, m_id);
	int rc = sqlite3_step(m_stmtSelectChildren);
	while (rc == SQLITE_ROW) {
		int id = sqlite3_column_int(m_stmtSelectChildren, 0);
		std::string miStr = (const char*)sqlite3_column_text(m_stmtSelectChildren, 1);
		ret.push_back(
			std::shared_ptr<NodeInterfaceSqlite>(
				new NodeInterfaceSqlite(MoveInput(miStr), id, m_stmtSelectChildren, m_stmtAddChild, m_stmtDelete)
			)
		);
		rc = sqlite3_step(m_stmtSelectChildren);
	}
	return ret;
}

void NodeInterfaceSqlite::addChild(MoveInput mi)
{
	sqlite3_clear_bindings(m_stmtAddChild);
	sqlite3_reset(m_stmtAddChild);
	std::string miStr = mi.getSaveStr();
	sqlite3_bind_text(m_stmtAddChild, 1, miStr.c_str(), miStr.size(), 0);
	sqlite3_bind_int(m_stmtAddChild, 2, m_id);
	sqlite3_step(m_stmtAddChild);
}

void NodeInterfaceSqlite::deleteThisNode()
{
	sqlite3_clear_bindings(m_stmtDelete);
	sqlite3_reset(m_stmtDelete);
	sqlite3_bind_int(m_stmtDelete, 1, m_id);
	sqlite3_step(m_stmtDelete);
}

MoveInput NodeInterfaceSqlite::getMoveInput()
{
	return m_mi;
}