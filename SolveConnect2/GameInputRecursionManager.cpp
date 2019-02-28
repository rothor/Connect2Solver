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
	rc = sql.execute("CREATE TABLE Input(InputId INTEGER PRIMARY KEY ASC, PathId INTEGER, Direction TEXT, PrevInputId INTEGER);");
}

GameInputRecursionManager::~GameInputRecursionManager()
{

	sql.finalize();
	sql.closeDb();
	DeleteFile(L"input.sqlite");
}

#include "Timer.h"
#include "HighResolutionTimer.h"
bool GameInputRecursionManager::recurse(GameInputRecursionNodeInfo& gii, MoveInputRow mir)
{
	if (gii.solutionFound)
		return false; // It doesn't really matter what we return here,
					  // as long as 'recurse' isn't manually called again.
					  // Once a solution is found, 'recurse' should not be called again.

	if (mir.rowId != 0)
		gii.gi.miArr.push_back(mir.mi);

	std::list<MoveInputRow> mirArr;

	// Query table and populate mirArr
	// --start sql
	HighResolutionTimer timer; // #####
	timer.reset(); // #####
	int rc = sqlite3_bind_int(m_stmtSelect, 1, mir.rowId);
	rc = sqlite3_step(m_stmtSelect);
	while (rc == SQLITE_ROW) {
		int inputId = sqlite3_column_int(m_stmtSelect, 0);
		int pathId = sqlite3_column_int(m_stmtSelect, 1);
		std::string direction = (const char*)sqlite3_column_text(m_stmtSelect, 2);
		mirArr.push_back(MoveInputRow(pathId, direction, inputId));
		rc = sqlite3_step(m_stmtSelect);
	}
	sqlite3_clear_bindings(m_stmtSelect);
	sqlite3_reset(m_stmtSelect);
	Timer::addTime(std::string("Select"), timer.readMicro()); // #####
	// --end sql

	if (mirArr.size() == 0) {
		addValidMoves(gii, mir, mirArr);
	}
	else {
		for (auto iter = mirArr.begin(); iter != mirArr.end(); ) {
			bool nextNodeHasValidBranches = recurse(gii, *iter);
			if (!nextNodeHasValidBranches) {
				// --start sql
				HighResolutionTimer timer; // #####
				timer.reset(); // #####
				int rc = sqlite3_bind_int(m_stmtDelete, 1, (*iter).rowId);
				rc = sqlite3_step(m_stmtDelete);
				sqlite3_clear_bindings(m_stmtDelete);
				sqlite3_reset(m_stmtDelete);
				Timer::addTime(std::string("Delete"), timer.readMicro()); // #####
				// --end sql
				iter = mirArr.erase(iter);
			}
			else
				iter++;
		}
	}

	if (mir.rowId != 0)
		gii.gi.miArr.pop_back();
		
	bool thisNodeHasValidBranches = mirArr.size() > 0;
	return thisNodeHasValidBranches;
}

void GameInputRecursionManager::addValidMoves(GameInputRecursionNodeInfo& gii, MoveInputRow& mir, std::list<MoveInputRow>& mirArr)
{
	//gii.game.reset();
	//gii.game.moveAll(gii.gi);

	int numOfPaths = gii.game.getNumOfPaths();
	std::list<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };
	for (int i = 0; i < numOfPaths; i++) {
		for (Direction direction : directions) {
			MoveInput newMi = MoveInput(gii.game.getPathDisplayId(i), direction);

			HighResolutionTimer timer; // #####
			timer.reset(); // #####
			gii.game.reset();
			gii.game.moveAll(gii.gi);
			Timer::addTime(std::string("MoveAll"), timer.readMicro()); // #####

			bool moveIsValid = gii.game.move(newMi);
			gii.movesEvaluated++; // for display
			if (moveIsValid) {
				gii.movesEvaluatedValid++; // for display

				std::string id = gii.game.getIdStr();
				bool isUnique = gii.idManager.addIdIsUnique(id);
				if (!isUnique) { // If game id already exists
					//gii.game.undo();
					continue;
				}

				if (gii.game.isSolved()) {
					gii.solutionFound = true;
					gii.solution = gii.gi;
					gii.solution.miArr.push_back(newMi);
					return; // We found the solution, so we don't need to do any more computation.
				}

				// --start sql
				HighResolutionTimer timer; // #####
				timer.reset(); // #####
				int rc = sqlite3_bind_int(m_stmtInsert, 1, newMi.pathId);
				std::string dirStr(1, directionToChar(newMi.direction));
				rc = sqlite3_bind_text(m_stmtInsert, 2, dirStr.c_str(), dirStr.size(), 0);
				rc = sqlite3_bind_int(m_stmtInsert, 3, mir.rowId);
				rc = sqlite3_step(m_stmtInsert);
				sqlite3_clear_bindings(m_stmtInsert);
				sqlite3_reset(m_stmtInsert);
				int rowId = 0; //sqlite3_last_insert_rowid(sql.m_sql);
				Timer::addTime(std::string("Insert"), timer.readMicro()); // #####
				// --end sql
				mirArr.push_back(MoveInputRow(newMi.pathId, std::string(1, directionToChar(newMi.direction)), rowId));
				gii.numBranches++;
				//gii.game.undo(); // Only undo if the move was valid.
			}
		}
	}
}

void GameInputRecursionManager::beginQuerying()
{
	sql.execute("BEGIN TRANSACTION;");
	std::string stmt = "SELECT InputId, PathId, Direction FROM Input WHERE PrevInputId = ?;";
	int rc = sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), -1, &(m_stmtSelect), 0);
	stmt = "INSERT INTO Input (PathId, Direction, PrevInputId) VALUES (?, ?, ?);";
	rc = sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), -1, &(m_stmtInsert), 0);
	stmt = "DELETE FROM Input WHERE InputId = ?;";
	rc = sqlite3_prepare_v2(sql.m_sql, stmt.c_str(), -1, &(m_stmtDelete), 0);
}

void GameInputRecursionManager::endQuerying()
{
	int rc = sqlite3_finalize(m_stmtSelect);
	m_stmtSelect = 0;
	rc = sqlite3_finalize(m_stmtInsert);
	m_stmtInsert = 0;
	rc = sqlite3_finalize(m_stmtDelete);
	m_stmtInsert = 0;
	sql.execute("END TRANSACTION;");
}