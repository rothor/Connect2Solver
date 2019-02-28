#include "GameInputManager.h"

#include "Timer.h"
#include "HighResolutionTimer.h"


GameInputManager::GameInputManager()
{
	DeleteFile(L"input.sqlite"); // Make sure database starts out fresh
	int rc = sqlite3_open("input.sqlite", &(sql.m_sql));
	rc = sql.execute("PRAGMA synchronous = OFF;"); // faster
	rc = sql.execute("PRAGMA JOURNAL_MODE = OFF;"); // faster
	rc = sql.execute("CREATE TABLE Input(InputId INTEGER PRIMARY KEY ASC, GameInput TEXT);");
}

GameInputManager::~GameInputManager()
{
	// delete sqlite file?
}

void GameInputManager::getRows()
{
	std::string stmt = "SELECT max(InputId) FROM Input;";
	int rc = sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), stmt.size(), &(sql.m_stmt), 0);
	rc = sqlite3_step(sql.m_stmt);
	m_max = sqlite3_column_int(sql.m_stmt, 0);
	rc = sqlite3_finalize(sql.m_stmt);
	sql.m_stmt = 0;
	if (m_max != 0) {
		HighResolutionTimer timer; // ##### for timing #####
		timer.reset(); // ##### for timing #####
		stmt = "SELECT GameInput FROM Input WHERE InputId <= " + std::to_string(m_max) + ";";
		sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), stmt.size(), &(sql.m_stmt), 0);
		Timer::addTime(std::string("Select"), timer.readMicro()); // ##### for timing #####
	}
}

bool GameInputManager::nextRow(GameInput& gi)
{
	if (m_max == 0) {
		gi = GameInput();
		m_max = -1;
		return true;
	}
	else if (m_max == -1) {
		sqlite3_finalize(sql.m_stmt);
		//sql.m_stmt = 0;
		return false;
	}

	int rc = sqlite3_step(sql.m_stmt);
	if (rc == SQLITE_ROW) {
		std::string giStr = (const char*)sqlite3_column_text(sql.m_stmt, 0);
		gi = GameInput(giStr);
		return true;
	}
	else {
		sqlite3_finalize(sql.m_stmt);
		//sql.m_stmt = 0;
		return false;
	}	
}

void GameInputManager::addGameInput(GameInput& gi)
{
	HighResolutionTimer timer; // ##### for timing #####
	timer.reset(); // ##### for timing #####
	std::string giStr = gi.getSaveString();
	int rc = sqlite3_bind_text(m_stmtInsert, 1, giStr.c_str(), giStr.size(), 0);
	rc = sqlite3_step(m_stmtInsert);
	sqlite3_clear_bindings(m_stmtInsert);
	sqlite3_reset(m_stmtInsert);
	Timer::addTime(std::string("Insert"), timer.readMicro()); // ##### for timing #####
}

void GameInputManager::removePrevGameInputs()
{
	HighResolutionTimer timer; // ##### for timing #####
	timer.reset(); // ##### for timing #####
	std::string stmt = "DELETE FROM Input WHERE InputId <= " + std::to_string(m_max) + ";";
	sql.execute(stmt);
	Timer::addTime(std::string("Delete"), timer.readMicro()); // ##### for timing #####
}

void GameInputManager::beginQuerying()
{
	sql.execute("BEGIN TRANSACTION;");
	std::string stmt = "INSERT INTO Input (GameInput) VALUES (?);";
	int rc = sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), stmt.size(), &m_stmtInsert, 0);
}

void GameInputManager::endQuerying()
{
	int rc = sqlite3_finalize(m_stmtInsert);
	//m_stmtInsert = 0;
	sql.execute("END TRANSACTION;");
}

void GameInputManager::clear()
{
	sql.execute("DELETE FROM Input;");
}