#include "GameInputManager.h"
#include <Windows.h>


int GameInputManager::instanceCount = 1;

GameInputManager::GameInputManager(int anInt) :
	m_finalized(false)
{
	m_fileName = std::string("input" + std::to_string(instanceCount++) + ".sqlite");
	DeleteFileA(m_fileName.c_str());
	int rc = sqlite3_open(m_fileName.c_str(), &(sql.m_sql));
	std::string stmt = "PRAGMA synchronous = OFF;"; // faster
	rc = sqlite3_exec(sql.m_sql, stmt.c_str(), 0, 0, 0);
	stmt = "PRAGMA JOURNAL_MODE = OFF;"; // faster
	rc = sqlite3_exec(sql.m_sql, stmt.c_str(), 0, 0, 0);
	stmt = "CREATE TABLE Input(InputId INTEGER PRIMARY KEY ASC, GameInput TEXT);";
	rc = sqlite3_exec(sql.m_sql, stmt.c_str(), 0, 0, 0);
}

GameInputManager::~GameInputManager()
{
	int rc = sqlite3_close(sql.m_sql);
	DeleteFileA(m_fileName.c_str());
}

void GameInputManager::getRows()
{
	std::string stmt = "SELECT max(InputId) FROM Input;";
	int rc = sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), stmt.size(), &(sql.m_stmt), 0);
	rc = sqlite3_step(sql.m_stmt);
	m_max = sqlite3_column_int(sql.m_stmt, 0);
	rc = sqlite3_finalize(sql.m_stmt);
	m_finalized = true;
	if (m_max != 0) {
		stmt = "SELECT GameInput FROM Input WHERE InputId <= " + std::to_string(m_max) + ";";
		sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), stmt.size(), &(sql.m_stmt), 0);
		m_finalized = false;
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
		m_finalized = true;
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
		m_finalized = true;
		return false;
	}	
}

void GameInputManager::addGameInput(GameInput& gi)
{
	std::string giStr = gi.getSaveString();
	int rc = sqlite3_bind_text(m_stmtInsert, 1, giStr.c_str(), giStr.size(), 0);
	rc = sqlite3_step(m_stmtInsert);
	rc = sqlite3_clear_bindings(m_stmtInsert);
	rc = sqlite3_reset(m_stmtInsert);
}

void GameInputManager::removePrevGameInputs()
{
	std::string stmt = "DELETE FROM Input WHERE InputId <= " + std::to_string(m_max) + ";";
	int rc = sqlite3_exec(sql.m_sql, stmt.c_str(), 0, 0, 0);
}

void GameInputManager::beginQuerying()
{
	std::string stmt = "BEGIN TRANSACTION;";
	int rc = sqlite3_exec(sql.m_sql, stmt.c_str(), 0, 0, 0);
	stmt = "INSERT INTO Input (GameInput) VALUES (?);";
	rc = sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), stmt.size(), &m_stmtInsert, 0);
}

void GameInputManager::endQuerying()
{
	int rc = sqlite3_finalize(m_stmtInsert);
	if (!m_finalized)
		rc = sqlite3_finalize(sql.m_stmt);
	std::string stmt = "END TRANSACTION;";
	rc = sqlite3_exec(sql.m_sql, stmt.c_str(), 0, 0, 0);
}
