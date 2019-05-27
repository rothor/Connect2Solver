#pragma once
#include "Sqlite.h"
#include "GameInput.h"


class GameInputManager
{
public:
	GameInputManager();
	~GameInputManager();

	void getRows();
	bool nextRow(GameInput& gi);
	void addGameInput(GameInput& gi);
	void removePrevGameInputs();
	void beginQuerying();
	void endQuerying();

protected:
	Sqlite sql;
	sqlite3_stmt* m_stmtInsert;
	int m_max;
	bool m_finalized;
};

