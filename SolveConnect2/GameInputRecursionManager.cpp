#include <Windows.h>
#include <list>
#include "GameInputRecursionManager.h"


void deleteFile(const wchar_t* file)
{
	DeleteFile(file);
	DWORD attrib = GetFileAttributes(file);
	while (attrib != INVALID_FILE_ATTRIBUTES) {
		attrib = GetFileAttributes(file);
		Sleep(1);
	}
}

GameInputRecursionManager::GameInputRecursionManager()
{
	DeleteFile(L"input.sqlite"); // Make sure database starts out fresh
	int rc = sqlite3_open("input.sqlite", &(sql.m_sql)); // This should probably have some error checking, but I don't want to right now.
	rc = sql.execute("PRAGMA synchronous = OFF;");
	rc = sql.execute("PRAGMA JOURNAL_MODE = OFF;");
	//rc = sql.execute("PRAGMA LOCKING_MODE = EXCLUSIVE;");
	rc = sql.execute("CREATE TABLE Input(InputId INTEGER PRIMARY KEY ASC, GameInput TEXT);");
}

GameInputRecursionManager::~GameInputRecursionManager()
{
	sql.finalize();
	sql.closeDb();
	DeleteFile(L"input.sqlite");
}

#include "Timer.h"
#include "HighResolutionTimer.h"
void GameInputRecursionManager::recurse(GameInputRecursionNodeInfo& gii)
{
	std::string stmt = "SELECT max(InputId) FROM Input;";
	sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), stmt.size(), &(sql.m_stmt), 0);
	int rc = sqlite3_step(sql.m_stmt);
	int max = sqlite3_column_int(sql.m_stmt, 0);
	sqlite3_finalize(sql.m_stmt);
	sql.m_stmt = 0;
	if (max != 0) {
		HighResolutionTimer timer; // #####
		timer.reset(); // #####
		stmt = "SELECT GameInput FROM Input WHERE InputId <= " + std::to_string(max) + ";";
		sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), stmt.size(), &(sql.m_stmt), 0);
		int rc = sqlite3_step(sql.m_stmt);
		Timer::addTime(std::string("Select"), timer.readMicro()); // #####
		while (rc == SQLITE_ROW) {
			std::string giStr = (const char*)sqlite3_column_text(sql.m_stmt, 0);
			GameInput gi(giStr);
			addValidMoves(gii, gi);
			if (gii.solutionFound)
				break;
			timer.reset(); // #####
			rc = sqlite3_step(sql.m_stmt);
			Timer::addTime(std::string("Select"), timer.readMicro()); // #####
		}
		sqlite3_finalize(sql.m_stmt);
		sql.m_stmt = 0;

		timer.reset(); // #####
		stmt = "DELETE FROM Input WHERE InputId <= " + std::to_string(max) + ";";
		sql.execute(stmt);
		Timer::addTime(std::string("Delete"), timer.readMicro()); // #####
	}
	else {
		GameInput gi;
		addValidMoves(gii, gi);
	}
}

#include <iostream>
void GameInputRecursionManager::addValidMoves(GameInputRecursionNodeInfo& gii, GameInput& gi)
{
	HighResolutionTimer timer; // #####
	timer.reset(); // #####
	//gii.game.reset();
	//gii.game.moveAll(gi);
	//Timer::addTime(std::string("MoveAll"), timer.readMicro()); // #####

	int numOfPaths = gii.game.getNumOfPaths();
	std::list<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };
	for (int i = 0; i < numOfPaths; i++) {
		for (Direction direction : directions) {
			MoveInput newMi = MoveInput(gii.game.getPathDisplayId(i), direction);

			timer.reset(); // #####
			gii.game.reset();
			gii.game.moveAll(gi);
			Timer::addTime(std::string("MoveAll"), timer.readMicro()); // #####

			timer.reset(); // #####
			bool moveIsValid = gii.game.move(newMi);
			Timer::addTime(std::string("MoveAll"), timer.readMicro()); // #####

			gii.movesEvaluated++; // for display
			if (moveIsValid) {
				gii.movesEvaluatedValid++; // for display

				std::string id = gii.game.getIdStr();
				bool isUnique = gii.idManager.addIdIsUnique(id);
				if (!isUnique) { // If game id already exists
					timer.reset(); // #####
					//gii.game.undo();
					//Timer::addTime(std::string("MoveAll"), timer.readMicro()); // #####
					continue;
				}

				if (gii.game.isSolved()) {
					gii.solutionFound = true;
					gii.solution = gi;
					gii.solution.miArr.push_back(newMi);
					return; // We found the solution, so we don't need to do any more computation.
				}

				// --start sql
				HighResolutionTimer timer; // #####
				timer.reset(); // #####
				gi.miArr.push_back(newMi);
				std::string giStr = gi.getSaveString();
				gi.miArr.pop_back();
				int rc = sqlite3_bind_text(m_stmtInsert, 1, giStr.c_str(), giStr.size(), 0);
				rc = sqlite3_step(m_stmtInsert);
				sqlite3_clear_bindings(m_stmtInsert);
				sqlite3_reset(m_stmtInsert);
				Timer::addTime(std::string("Insert"), timer.readMicro()); // #####
				// --end sql
				gii.numBranches++;
				timer.reset(); // #####
				//gii.game.undo(); // Only undo if the move was valid.
				//Timer::addTime(std::string("MoveAll"), timer.readMicro()); // #####
			}
		}
	}
}

void GameInputRecursionManager::beginQuerying()
{
	sql.execute("BEGIN TRANSACTION;");
	std::string stmt = "INSERT INTO Input (GameInput) VALUES (?);";
	int rc = sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), stmt.size(), &m_stmtInsert, 0);
}

void GameInputRecursionManager::endQuerying()
{
	int rc = sqlite3_finalize(m_stmtInsert);
	m_stmtInsert = 0;
	sql.execute("END TRANSACTION;");
}