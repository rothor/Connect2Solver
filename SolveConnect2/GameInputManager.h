#pragma once
#include "sqlite3.h"
#include "GameInput.h"
#include <string>


class GameInputManager
{
public:
	GameInputManager();
	~GameInputManager();

	void getRows();
	bool nextRow(GameInput& gi);
	void addGameInput(GameInput& gi);
	void removePrevGameInputs();

protected:
	sqlite3* m_conn;
	sqlite3_stmt* m_stmtSelectMax;
	sqlite3_stmt* m_stmtSelectLess;
	sqlite3_stmt* m_stmtInsert;
	sqlite3_stmt* m_stmtDelete;
	int m_max;
	std::string m_dbFileName;
};
