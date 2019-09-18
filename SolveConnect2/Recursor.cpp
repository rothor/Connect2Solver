#include "Recursor.h"


void Recursor::addNextInputs(RecursionStruct& rs)
{
	auto node = rs.mit->getStartNode();
	recurse(rs, node, true);
}

bool Recursor::recurse(RecursionStruct& rs, std::shared_ptr<NodeInterface>& node, bool first)
{
	if (!first)
		rs.gi.miArr.push_back(node->getMoveInput());

	std::list<std::shared_ptr<NodeInterface>> childNodes = node->getChildren();
	bool thisNodeHasValidBranches = false;
	if (childNodes.size() == 0) {
		thisNodeHasValidBranches = addValidMoves(rs, node);
	}
	else {
		for (auto it = childNodes.begin(); it != childNodes.end(); ) {
			bool nextNodeHasValidBranches = recurse(rs, *it);
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
		rs.gi.miArr.pop_back();
	
	return thisNodeHasValidBranches;
}

bool Recursor::addValidMoves(RecursionStruct& rs, std::shared_ptr<NodeInterface>& node)
{
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	rs.game = rs.gameStart;
	rs.game.moveAll(rs.gi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
	
	bool movesWereAdded = false;
	std::list<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };

	if (rs.solveEndPos) {
		std::vector<int>* pathList = rs.game.getPathIdsOrderedByLength();
		for (auto it = pathList->begin(); it != pathList->end(); it++) {
			if (rs.game.pathIsFull(*it)) {
				continue;
			}
			for (Direction direction : directions) {
				MoveInput newMi = MoveInput(rs.game.getPathDisplayId(*it), direction, true);
				bool moveAdded = addMove(rs, node, newMi);
				if (moveAdded)
					movesWereAdded = true;
			}
			break;
		}
	}
	else {
		int numOfPaths = rs.game.getNumOfPaths();
		for (int i = 0; i < numOfPaths; i++) {
			for (Direction direction : directions) {
				MoveInput newMi = MoveInput(rs.game.getPathDisplayId(i), direction);
				bool moveAdded = addMove(rs, node, newMi);
				if (moveAdded)
					movesWereAdded = true;
			}
		}
	}
	
	return movesWereAdded;
}

bool Recursor::addMove(RecursionStruct& rs, std::shared_ptr<NodeInterface>& node, MoveInput& mi)
{
	bool moveAdded = false;
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	bool moveIsValid = rs.game.move(mi);
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
	rs.movesEvaluated++; // for display

	if (!moveIsValid)
		return moveAdded;
	rs.movesEvaluatedValid++; // for display

	bool isSolved = rs.si->isSolved(rs.game);
	if (isSolved) {
		rs.solutionFound = true;
		GameInput solutionGi = rs.gi;
		solutionGi.miArr.push_back(mi);
		rs.addSolution(solutionGi);
		#ifdef doBenchmarking
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		#endif
		rs.game.undo(); // Only undo if the move was valid.
		#ifdef doBenchmarking
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		#endif
		return moveAdded;
	}

	#ifdef doBenchmarking
	Benchmarker::resetTimer("getGameId"); // ## for benchmarking ##
	#endif
	std::string id = rs.game.getIdStr();
	#ifdef doBenchmarking
	Benchmarker::addTime("getGameId"); // ## for benchmarking ##
	Benchmarker::resetTimer("idIsUnique"); // ## for benchmarking ##
	#endif
	bool isUnique = rs.idManager->addIdIsUnique(id);
	#ifdef doBenchmarking
	Benchmarker::addTime("idIsUnique"); // ## for benchmarking ##
	#endif
	if (!isUnique) { // If game id already exists
		#ifdef doBenchmarking
		Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
		#endif
		rs.game.undo(); // Only undo if the move was valid.
		#ifdef doBenchmarking
		Benchmarker::addTime("moveAll"); // ## for benchmarking ##
		#endif
		return moveAdded;
	}
	
	#ifdef doBenchmarking
	Benchmarker::resetTimer("addInput"); // ## for benchmarking ##
	#endif
	node->addChild(mi);
	#ifdef doBenchmarking
	Benchmarker::addTime("addInput"); // ## for benchmarking ##
	#endif
	moveAdded = true;
	rs.numBranches++; // for display
	
	#ifdef doBenchmarking
	Benchmarker::resetTimer("moveAll"); // ## for benchmarking ##
	#endif
	rs.game.undo(); // Only undo if the move was valid.
	#ifdef doBenchmarking
	Benchmarker::addTime("moveAll"); // ## for benchmarking ##
	#endif
	return moveAdded;
}