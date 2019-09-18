#pragma once
#include "RecursionStruct.h"
#include "Connect2.h"
#include "SolveOptions.h"


class Connect2Solver
{
public:
	Connect2Solver() = delete;

	static void solve(Connect2 game, SolveOptions options);
};
