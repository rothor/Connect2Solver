#pragma once
#include "RecursionStruct.h"
#include "MoveInputRow.h"


class Connect2Solver
{
public:
	Connect2Solver(); // delete this later and make class static

	void solve(RecursionStruct& gii);

protected:
	void recurse(RecursionStruct& gii);
	void addValidMoves(RecursionStruct& gii, GameInput& gi);
	void addMove(RecursionStruct& gii, GameInput& gi, MoveInput& mi);
	void cleanup(RecursionStruct& gii);
};
