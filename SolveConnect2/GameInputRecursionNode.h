#pragma once
#include <list>
#include <memory>


class GameInputRecursionNode
{
public:
	GameInputRecursionNode();

	int addMoves(GameInput& gi);
	
public:
	std::list<std::unique_ptr<GameInputRecursionNode>> moveArr;
};
