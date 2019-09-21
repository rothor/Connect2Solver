#include "Recursor.h"
#include "SolutionInputManager.h"
#include "IdManagerSqlite.h"
#include "IdManagerMemory.h"
#include "MoveInputTreeMemory.h"
#include "MoveInputTreeSqlite.h"
#include "MoveInputTreeSqliteFast.h"
#include "SolvedInterfaceComplete.h"
#include "SolvedInterfaceHash.h"
#include "Misc.h"

#define doBenchmarking


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
			new MoveInputTreeSqliteFast()
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
	#ifdef doBenchmarking
	m_bmFile = std::ofstream("benchmarks.txt");
	m_bmFile.clear();
	m_bmFile << "[Depth - Paths - Evaluated]\n\n";
	#endif
}

Recursor::~Recursor()
{
	#ifdef doBenchmarking
	m_bmFile.close();
	#endif
}

void Recursor::addNextInputs()
{
	m_numBranches = 0;
	auto node = m_mit->getStartNode();

	#ifdef doBenchmarking
	m_bm.clearAllTimes();
	m_bm.resetTimer("TotalTime");
	#endif
	recurse(&*node, true);
	#ifdef doBenchmarking
	m_bm.addTime("TotalTime");
	#endif

	m_depth++;

	#ifdef doBenchmarking
	m_bmFile << Misc::formatIntWithCommas(m_depth) << " - " <<
		Misc::formatIntWithCommas(m_numBranches) << " - " <<
		Misc::formatIntWithCommas(m_movesEvaluated) << "\n";
	
	m_bmFile << "This recursion:\n";
	m_bmFile << m_bm.getStr("TotalTime") << "\n";
	std::list<std::string> pieces{ "Connect2::moveAll", "Connect2::getGameId", "IdManager::idIsUnique",
		"NodeInterface::getNextChild", "NodeInterface::deleteCurrentChild", "NodeInterface::addChild" };
	m_bmFile << m_bm.getPieChartStr(pieces) << "\n";

	m_bmFile << "Running total:\n";
	m_bmFile << m_bm.getTotalStr("TotalTime") << "\n";
	m_bmFile << m_bm.getPieChartTotalStr(pieces) << "\n\n";
	#endif
}

bool Recursor::recurse(NodeInterface* node, bool first)
{
	std::list<MoveInput>::iterator it;
	if (!first) {
		GameInput gi = node->getGameInput();
		it = m_gi.miArr.insert(m_gi.miArr.end(), gi.miArr.begin(), gi.miArr.end());
	}

	int childCountBefore = 0;
	int childCountAfter = 0;
	std::shared_ptr<NodeInterface> childNode;
	while (true) {
		#ifdef doBenchmarking
		m_bm.resetTimer("NodeInterface::getNextChild");
		#endif
		bool hasAnotherChild = node->getNextChild(childNode);
		#ifdef doBenchmarking
		m_bm.addTime("NodeInterface::getNextChild");
		#endif
		if (!hasAnotherChild)
			break;
		childCountBefore++;
		childCountAfter++;
		bool nextNodeHasValidBranches = recurse(&*childNode);
		if (!nextNodeHasValidBranches) {
			#ifdef doBenchmarking
			m_bm.resetTimer("NodeInterface::deleteCurrentChild");
			#endif
			node->deleteCurrentChild();
			#ifdef doBenchmarking
			m_bm.addTime("NodeInterface::deleteCurrentChild");
			#endif
			childCountAfter--;
		}
	}

	bool thisNodeHasValidBranches = false;
	if (childCountBefore == 0)
		thisNodeHasValidBranches = addValidMoves(node);
	else
		thisNodeHasValidBranches = childCountAfter > 0;

	if (!first)
		m_gi.miArr.erase(it, m_gi.miArr.end());
	
	return thisNodeHasValidBranches;
}

bool Recursor::addValidMoves(NodeInterface* node)
{
	#ifdef doBenchmarking
	m_bm.resetTimer("Connect2::moveAll");
	#endif
	m_game = m_gameStart;
	m_game.moveAll(m_gi);
	#ifdef doBenchmarking
	m_bm.addTime("Connect2::moveAll");
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

bool Recursor::addMove(NodeInterface* node, MoveInput& mi)
{
	m_movesEvaluated++;
	bool moveAdded = false;
	#ifdef doBenchmarking
	m_bm.resetTimer("Connect2::moveAll");
	#endif
	bool moveIsValid = m_game.move(mi);
	#ifdef doBenchmarking
	m_bm.addTime("Connect2::moveAll");
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
		m_bm.resetTimer("Connect2::moveAll");
		#endif
		m_game.undo(); // Only undo if the move was valid.
		#ifdef doBenchmarking
		m_bm.addTime("Connect2::moveAll");
		#endif
		return moveAdded;
	}

	if (!m_solveEndPos) {
		#ifdef doBenchmarking
		m_bm.resetTimer("Connect2::getGameId");
		#endif
		std::string id = m_game.getIdStr();
		#ifdef doBenchmarking
		m_bm.addTime("Connect2::getGameId");
		m_bm.resetTimer("IdManager::idIsUnique");
		#endif
		bool isUnique = m_idManager->addIdIsUnique(id);
		#ifdef doBenchmarking
		m_bm.addTime("IdManager::idIsUnique");
		#endif
		if (!isUnique) { // If game id already exists
			#ifdef doBenchmarking
			m_bm.resetTimer("Connect2::moveAll");
			#endif
			m_game.undo(); // Only undo if the move was valid.
			#ifdef doBenchmarking
			m_bm.addTime("Connect2::moveAll");
			#endif
			return moveAdded;
		}
	}
	m_movesEvaluatedUnique++;
	
	#ifdef doBenchmarking
	m_bm.resetTimer("NodeInterface::addChild");
	#endif
	node->addChild(mi);
	#ifdef doBenchmarking
	m_bm.addTime("NodeInterface::addChild");
	#endif
	moveAdded = true;
	m_numBranches++;
	
	#ifdef doBenchmarking
	m_bm.resetTimer("Connect2::moveAll");
	#endif
	m_game.undo(); // Only undo if the move was valid.
	#ifdef doBenchmarking
	m_bm.addTime("Connect2::moveAll");
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