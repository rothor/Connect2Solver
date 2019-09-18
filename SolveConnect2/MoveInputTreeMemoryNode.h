#pragma once
#include <memory>
#include <list>
#include "MoveInput.h"


class MoveInputTreeMemoryNode
{
public:
	MoveInputTreeMemoryNode(MoveInput mi);
	void addChild(MoveInput mi);

public:
	MoveInput m_mi;
	std::list<std::shared_ptr<MoveInputTreeMemoryNode>> m_childNodes;
};