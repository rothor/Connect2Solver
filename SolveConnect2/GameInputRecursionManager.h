#pragma once
#include "GameInputRecursionNodeInfo.h"
#include "Sqlite.h"
#include "MoveInputRow.h"


class GameInputRecursionManager
{
public:
	GameInputRecursionManager();
	~GameInputRecursionManager();

	bool recurse(GameInputRecursionNodeInfo& gii, MoveInputRow mir = MoveInputRow());
	void beginQuerying();
	void endQuerying();

protected:
	void addValidMoves(GameInputRecursionNodeInfo& gii, MoveInputRow& mir, std::list<MoveInputRow>& mirArr);
	Sqlite sql;
	sqlite3_stmt* m_stmtSelect;
	sqlite3_stmt* m_stmtInsert;
	sqlite3_stmt* m_stmtDelete;
};
