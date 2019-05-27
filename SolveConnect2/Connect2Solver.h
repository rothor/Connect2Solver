#pragma once
#include "RecursionStruct.h"
#include "Connect2.h"


class Connect2Solver
{
public:
	Connect2Solver() = delete;

	static void solve(std::string fileName, GameInput gi);
	static void solveCustom(std::string fileName, GameInput gi, std::string endHash);

protected:
	static void recurse(RecursionStruct& gii);
	static void addValidMoves(RecursionStruct& gii, GameInput& gi);
	static void addMove(RecursionStruct& gii, GameInput& gi, MoveInput& mi);

	static void recurseCustom(RecursionStruct& gii, std::string& endHash);
	static void addValidMovesCustom(RecursionStruct& gii, GameInput& gi, std::string& endHash);
	static void addMoveCustom(RecursionStruct& gii, GameInput& gi, MoveInput& mi, std::string& endHash);
};
