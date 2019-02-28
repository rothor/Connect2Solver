#pragma once
#include "GameInputRecursionNodeInfo.h"
#include "Sqlite.h"
#include "MoveInputRow.h"


class GameInputRecursionManager
{
public:
	GameInputRecursionManager();
	~GameInputRecursionManager();

	void recurse(GameInputRecursionNodeInfo& gii);
	void beginQuerying();
	void endQuerying();

protected:
	void addValidMoves(GameInputRecursionNodeInfo& gii, GameInput& gi);
	Sqlite sql;
	sqlite3_stmt* m_stmtInsert;
};
