#include "GameInputRecursionNode.h"


GameInputRecursionNode::GameInputRecursionNode(MoveInput move, bool isFirst) :
	move(move),
	isFirst(isFirst)
{

}

bool GameInputRecursionNode::recurse(GameInputRecursionNodeInfo& gii)
{
	if (gii.solutionFound)
		return false; // It doesn't really matter what we return here,
					  // as long as 'recurse' isn't manually called again.
					  // Once a solution is found, 'recurse' should not be called again.

	if (!isFirst)
		gii.gi.miArr.push_back(move);
	
	if (moveArr.size() == 0) {
		addValidMoves(gii);
	}
	else {
		for (auto iter = moveArr.begin(); iter != moveArr.end(); ) {
			bool nextNodeHasValidBranches = (*iter)->recurse(gii);
			if (!nextNodeHasValidBranches)
				iter = moveArr.erase(iter);
			else
				iter++;
		}
	}

	if (!isFirst)
		gii.gi.miArr.pop_back();

	bool thisNodeHasValidBranches = moveArr.size() > 0;
	return thisNodeHasValidBranches;
}

void GameInputRecursionNode::addValidMoves(GameInputRecursionNodeInfo& gii)
{
	//gii.game.reset();
	//gii.game.moveAll(gii.gi);

	int numOfPaths = gii.game.getNumOfPaths();
	std::list<Direction> directions{ Direction::down, Direction::up, Direction::left, Direction::right };
	for (int i = 0; i < numOfPaths; i++) {
		for (Direction direction : directions) {
			MoveInput newMi = MoveInput(gii.game.getPathDisplayId(i), direction);

			gii.game.reset();
			gii.game.moveAll(gii.gi);

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

				moveArr.push_back(
					std::unique_ptr<GameInputRecursionNode>(
						new GameInputRecursionNode(newMi, false)
					)
				);
				gii.numBranches++;
				//gii.game.undo(); // Only undo if the move was valid.
			}
		}
	}
}