#pragma once
#include <list>
#include <memory>
#include "GameInputRecursionNodeInfo.h"


class GameInputRecursionNode
{
public:
	GameInputRecursionNode(MoveInput move, bool isFirst = true);

	bool recurse(GameInputRecursionNodeInfo& gii);

protected:
	void addValidMoves(GameInputRecursionNodeInfo& gii);
	
protected:
	std::list<std::unique_ptr<GameInputRecursionNode>> moveArr;
	MoveInput move;
	bool isFirst;
};
