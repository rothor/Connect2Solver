#pragma once
#include <list>
#include <memory>
#include "MoveInput.h"


class NodeInterface
{
public:
	NodeInterface();
	virtual std::list<std::shared_ptr<NodeInterface>> getChildren() = 0;
	virtual void addChild(MoveInput mi) = 0;
	virtual void deleteThisNode() = 0;
	virtual MoveInput getMoveInput() = 0;
};