#include "NodeInterfaceSqlite.h"


NodeInterfaceSqlite::NodeInterfaceSqlite(const MoveInput& mi, int id, sqlite3_stmt* selectChildren, sqlite3_stmt* addChild, sqlite3_stmt* deletez) :
	m_mi(mi),
	m_id(id),
	m_stmtSelectChildren(selectChildren),
	m_stmtAddChild(addChild),
	m_stmtDelete(deletez),
	m_itInitialized(false),
	m_deleted(false)
{

}

NodeInterfaceSqlite::~NodeInterfaceSqlite()
{

}

bool NodeInterfaceSqlite::getNextChild(std::shared_ptr<NodeInterface>& node)
{
	if (m_itInitialized) {
		if (!m_deleted)
			++m_it;
	}
	else {
		sqlite3_clear_bindings(m_stmtSelectChildren);
		sqlite3_reset(m_stmtSelectChildren);
		sqlite3_bind_int(m_stmtSelectChildren, 1, m_id);
		int rc = sqlite3_step(m_stmtSelectChildren);
		while (rc == SQLITE_ROW) {
			int id = sqlite3_column_int(m_stmtSelectChildren, 0);
			std::string miStr = (const char*)sqlite3_column_text(m_stmtSelectChildren, 1);
			m_children.push_back(
				std::shared_ptr<NodeInterfaceSqlite>{
					new NodeInterfaceSqlite(MoveInput(miStr), id, m_stmtSelectChildren, m_stmtAddChild, m_stmtDelete)
				}
			);
			rc = sqlite3_step(m_stmtSelectChildren);
		}
		m_it = m_children.begin();
		m_itInitialized = true;
	}
	m_deleted = false;

	if (m_it == m_children.end())
		return false;
	else {
		node = *m_it;
		return true;
	}
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

void NodeInterfaceSqlite::deleteCurrentChild()
{
	sqlite3_clear_bindings(m_stmtDelete);
	sqlite3_reset(m_stmtDelete);
	sqlite3_bind_int(m_stmtDelete, 1, (*m_it)->getId());
	sqlite3_step(m_stmtDelete);
	m_it = m_children.erase(m_it);
	m_deleted = true;
}

GameInput NodeInterfaceSqlite::getGameInput()
{
	return m_mi;
}

int NodeInterfaceSqlite::getId()
{
	return m_id;
}