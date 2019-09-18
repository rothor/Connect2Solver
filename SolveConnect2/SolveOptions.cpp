#include "SolveOptions.h"


SolveOptions::SolveOptions(bool hashtableInSqlite, bool treeInSqlite, bool solveEndPos, bool solveToPos, std::string endHash) :
	hashtableInSqlite(hashtableInSqlite),
	treeInSqlite(treeInSqlite),
	solveEndPos(solveEndPos),
	solveToPos(solveToPos),
	endHash(endHash)
{

}