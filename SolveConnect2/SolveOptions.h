#pragma once
#include "Connect2.h"
#include <string>


class SolveOptions
{
public:
	SolveOptions(bool hashtableInSqlite, bool treeInSqlite, bool solveEndPos, bool solveToPos, std::string endHash = "");

public:
	bool hashtableInSqlite;
	bool treeInSqlite;
	bool solveEndPos;
	bool solveToPos;
	std::string endHash;
};