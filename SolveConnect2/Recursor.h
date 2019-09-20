#pragma once
#include <memory>
#include <list>
#include <string>
#include "GameInput.h"
#include "Connect2.h"
#include "IdManager.h"
#include "MoveInputTree.h"
#include "SolvedInterface.h"
#include "SolveOptions.h"
#include "NodeInterface.h"


class Recursor
{
public:
	Recursor(Connect2 game, SolveOptions so);
	void addNextInputs();
	std::string getDisplayStr();
	bool isSolved();
	bool solutionWasFound();
	int getNumBranches();
	int getNumMovesEvaluated();
	int getNumMovesEvaluatedValid();
	int getNumMovesEvaluatedUnique();
	int getDepth();

protected:
	bool recurse(NodeInterface* node, bool first = false);
	bool addValidMoves(NodeInterface* node);
	bool addMove(NodeInterface* node, MoveInput& mi);
	void addSolution(GameInput gi);

protected:
	Connect2 m_game;
	Connect2 m_gameStart;
	bool m_solutionFound;
	std::list<GameInput> m_solutionList;
	std::unique_ptr<IdManager> m_idManager;
	GameInput m_gi;
	std::unique_ptr<MoveInputTree> m_mit;
	std::unique_ptr<SolvedInterface> m_si;
	bool m_solveEndPos;

	int m_movesEvaluated;
	int m_movesEvaluatedValid;
	int m_movesEvaluatedUnique;
	int m_depth;
	int m_numBranches;
};