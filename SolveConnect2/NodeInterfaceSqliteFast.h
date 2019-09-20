#pragma once
#include "NodeInterface.h"
#include "sqlite3.h"
#include <list>


class NodeInterfaceSqliteFast : public NodeInterface
{
public:
	NodeInterfaceSqliteFast(const GameInput& gi,
		bool isStartNode,
		sqlite3_stmt* selectChildren,
		sqlite3_stmt* addChild,
		sqlite3_stmt* deletez,
		sqlite3_stmt* getMax);
	~NodeInterfaceSqliteFast();
	bool getNextChild(std::shared_ptr<NodeInterface>& node);
	void addChild(MoveInput mi);
	void deleteCurrentChild();
	GameInput getGameInput();

protected:
	GameInput m_gi;
	bool m_isStartNode;
	bool m_itInitialized;
	int m_max;
	sqlite3_stmt* m_stmtSelectChildren;
	sqlite3_stmt* m_stmtAddChild;
	sqlite3_stmt* m_stmtDelete;
	sqlite3_stmt* m_stmtGetMax;
};