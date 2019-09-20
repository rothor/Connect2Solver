#include "NodeInterfaceSqliteFast.h"


NodeInterfaceSqliteFast::NodeInterfaceSqliteFast(const GameInput& gi,
	bool isStartNode,
	sqlite3_stmt* selectChildren,
	sqlite3_stmt* addChild,
	sqlite3_stmt* deletez,
	sqlite3_stmt* getMax) :
	m_gi(gi),
	m_isStartNode(isStartNode),
	m_stmtSelectChildren(selectChildren),
	m_stmtAddChild(addChild),
	m_stmtDelete(deletez),
	m_stmtGetMax(getMax),
	m_itInitialized(false)
{

}

NodeInterfaceSqliteFast::~NodeInterfaceSqliteFast()
{
	if (m_isStartNode) {
		sqlite3_clear_bindings(m_stmtDelete);
		sqlite3_reset(m_stmtDelete);
		sqlite3_bind_int(m_stmtDelete, 1, m_max);
		sqlite3_step(m_stmtDelete);
	}
}

bool NodeInterfaceSqliteFast::getNextChild(std::shared_ptr<NodeInterface>& node)
{
	if (!m_isStartNode)
		return false;

	if (!m_itInitialized) {
		sqlite3_reset(m_stmtGetMax);
		sqlite3_step(m_stmtGetMax);
		m_max = sqlite3_column_int(m_stmtGetMax, 0);
		sqlite3_clear_bindings(m_stmtSelectChildren);
		sqlite3_reset(m_stmtSelectChildren);
		sqlite3_bind_int(m_stmtSelectChildren, 1, m_max);
		m_itInitialized = true;
	}
	int rc = sqlite3_step(m_stmtSelectChildren);
	if (rc == SQLITE_ROW) {
		std::string giStr = (const char*)sqlite3_column_text(m_stmtSelectChildren, 0);
		node = std::shared_ptr<NodeInterface>{
			new NodeInterfaceSqliteFast(
				GameInput(giStr), false, m_stmtSelectChildren, m_stmtAddChild, m_stmtDelete, m_stmtGetMax
			)
		};
		return true;
	}
	else
		return false;
}

void NodeInterfaceSqliteFast::addChild(MoveInput mi)
{
	sqlite3_clear_bindings(m_stmtAddChild);
	sqlite3_reset(m_stmtAddChild);
	m_gi.miArr.push_back(mi);
	std::string giStr = m_gi.getSaveString();
	sqlite3_bind_text(m_stmtAddChild, 1, giStr.c_str(), giStr.size(), 0);
	sqlite3_step(m_stmtAddChild);
	m_gi.miArr.pop_back();
}

void NodeInterfaceSqliteFast::deleteCurrentChild()
{
	
}

GameInput NodeInterfaceSqliteFast::getGameInput()
{
	return m_gi;
}