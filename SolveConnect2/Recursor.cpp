#include "Recursor.h"
#include "SolutionInputManager.h"
#include "IdManagerSqlite.h"
#include "IdManagerMemory.h"
#include "MoveInputTreeMemory.h"
#include "MoveInputTreeSqlite.h"
#include "SolvedInterfaceComplete.h"
#include "SolvedInterfaceHash.h"


Recursor::Recursor(Connect2 game, SolveOptions so) :
	m_game(game),
	m_gameStart(game),
	m_movesEvaluated(0),
	m_movesEvaluatedValid(0),
	m_movesEvaluatedUnique(0),
	m_depth(0),
	m_numBranches(0),
	m_solutionFound(false),
	m_solveEndPos(so.solveEndPos)
{
	if (so.hashtableInSqlite) {
		m_idManager = std::unique_ptr<IdManager>{
			new IdManagerSqlite()
		};
	}
	else {
		m_idManager = std::unique_ptr<IdManager>{
			new IdManagerMemory()
		};
	}

	if (so.treeInSqlite) {
		m_mit = std::unique_ptr<MoveInputTree>{
			new MoveInputTreeSqlite()
		};
	}
	else {
		m_mit = std::unique_ptr<MoveInputTree>{
			new MoveInputTreeMemory()
		};
	}

	if (so.solveToPos) {
		m_si = std::unique_ptr<SolvedInterface>{
			new SolvedInterfaceHash(so.endHash)
		};
	}
	else {
		m_si = std::unique_ptr<SolvedInterface>{
			new SolvedInterfaceComplete()
		};
	}

	m_idManager->addIdIsUnique(game.getIdStr());
}

void Recursor::addNextInputs()
{
	m_numBranches = 0;
	auto node = m_mit->getStartNode();
	recurse(node, true);
	m_depth++;
}

bool Recursor::recurse(std::shared_ptr<NodeInterface>& node, bool first)
{
	if (!first)
		m_gi.miArr.push_back(node->getMoveInput());

	std::list<std::shared_ptr<NodeInterface>> childNodes = node->getChildren();
	bool thisNodeHasValidBranches = false;
	if (childNodes.size() == 0) {
		thisNodeHasValidBranches = addValidMoves(node);
	}
	else {
		for (auto it = childNodes.begin(); it != childNodes.end(); ) {
			bool nextNodeHasValidBranches = recurse(*it);
			if (!nextNodeHasValidBranches) {
				(*it)->deleteThisNode();
				it = childNodes.erase(it);
			}
			else
				it++;
		}
		thisNodeHasValidBranches = childNodes.size() > 0;
	}

	if (!first)
		m_gi.miArr.pop_back();
	
	return thisNodeHasValidBranches;
}

bool Recursor::addValidMoves(std::shared_ptr<NodeInterface>& node)
{
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	m_game = m_gameStart;
	m_game.moveAll(m_gi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
	
	bool movesWereAdded = false;
	std::list<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };

	if (m_solveEndPos) {
		std::vector<int>* pathList = m_game.getPathIdsOrderedByLength();
		for (auto it = pathList->begin(); it != pathList->end(); it++) {
			if (m_game.pathIsFull(*it)) {
				continue;
			}
			for (Direction direction : directions) {
				MoveInput newMi = MoveInput(m_game.getPathDisplayId(*it), direction, true);
				bool moveAdded = addMove(node, newMi);
				if (moveAdded)
					movesWereAdded = true;
			}
			break;
		}
	}
	else {
		int numOfPaths = m_game.getNumOfPaths();
		for (int i = 0; i < numOfPaths; i++) {
			for (Direction direction : directions) {
				MoveInput newMi = MoveInput(m_game.getPathDisplayId(i), direction);
				bool moveAdded = addMove(node, newMi);
				if (moveAdded)
					movesWereAdded = true;
			}
		}
	}
	
	return movesWereAdded;
}

bool Recursor::addMove(std::shared_ptr<NodeInterface>& node, MoveInput& mi)
{
	m_movesEvaluated++;
	bool moveAdded = false;
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	bool moveIsValid = m_game.move(mi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif

	if (!moveIsValid)
		return moveAdded;
	m_movesEvaluatedValid++;

	bool isSolved = m_si->isSolved(m_game);
	if (isSolved) {
		m_solutionFound = true;
		GameInput solutionGi = m_gi;
		solutionGi.miArr.push_back(mi);
		addSolution(solutionGi);
		#ifdef doBenchmarking
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		#endif
		m_game.undo(); // Only undo if the move was valid.
		#ifdef doBenchmarking
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		#endif
		return moveAdded;
	}

	#ifdef doBenchmarking
	Benchmarker::resetTimer("getGameId"); // ## for benchmarking ##
	#endif
	std::string id = m_game.getIdStr();
	#ifdef doBenchmarking
	Benchmarker::addTime("getGameId"); // ## for benchmarking ##
	Benchmarker::resetTimer("idIsUnique"); // ## for benchmarking ##
	#endif
	bool isUnique = m_idManager->addIdIsUnique(id);
	#ifdef doBenchmarking
	Benchmarker::addTime("idIsUnique"); // ## for benchmarking ##
	#endif
	if (!isUnique) { // If game id already exists
		#ifdef doBenchmarking
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		#endif
		m_game.undo(); // Only undo if the move was valid.
		#ifdef doBenchmarking
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		#endif
		return moveAdded;
	}
	m_movesEvaluatedUnique++;
	
	#ifdef doBenchmarking
	Benchmarker::resetTimer("addInput"); // ## for benchmarking ##
	#endif
	node->addChild(mi);
	#ifdef doBenchmarking
	Benchmarker::addTime("addInput"); // ## for benchmarking ##
	#endif
	moveAdded = true;
	m_numBranches++; // for display
	
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	m_game.undo(); // Only undo if the move was valid.
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
	return moveAdded;
}

std::string Recursor::getDisplayStr()
{
	std::string ret;
	if (m_solutionFound) {
		ret += std::to_string(m_solutionList.size()) + " solution" + (m_solutionList.size() > 1 ? "s" : "") + " found in " + std::to_string(m_solutionList.begin()->miArr.size()) + " moves\n";
		int i = 1;
		for (auto it = m_solutionList.begin(); it != m_solutionList.end(); it++) {
			ret += "Solution " + std::to_string(i++) + ":\n";
			ret += it->getDisplayStr() + "\n";
			ret += "Save string: " + it->getSaveString();
			if (std::distance(it, m_solutionList.end()) > 1)
				ret += "\n\n";
		}
	}
	else {
		ret += "Solution not found.";
	}
	return ret;
}

void Recursor::addSolution(GameInput gi)
{
	SolutionInputManager sim(m_gameStart);
	for (auto moveIt = gi.miArr.begin(); moveIt != gi.miArr.end(); moveIt++) {
		sim.addMove(*moveIt);
	}
	// Check that new solution isn't the same as any of the current solutions
	GameInput newGi = sim.getSolution();
	for (auto solution : m_solutionList) {
		if (newGi.getSaveString() == solution.getSaveString())
			return;
	}
	// If not, add solution
	m_solutionList.push_back(newGi);
}

bool Recursor::isSolved()
{
	return m_si->isSolved(m_game);
}

bool Recursor::solutionWasFound()
{
	return m_solutionFound;
}

int Recursor::getNumBranches()
{
	return m_numBranches;
}

int Recursor::getNumMovesEvaluated()
{
	return m_movesEvaluated;
}

int Recursor::getNumMovesEvaluatedValid()
{
	return m_movesEvaluatedValid;
}

int Recursor::getNumMovesEvaluatedUnique()
{
	return m_movesEvaluatedUnique;
}

int Recursor::getDepth()
{
	return m_depth;
}