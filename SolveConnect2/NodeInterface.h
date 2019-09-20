#pragma once
#include <list>
#include <memory>
#include "GameInput.h"


class NodeInterface
{
public:
	NodeInterface();
	virtual bool getNextChild(std::shared_ptr<NodeInterface>& node) = 0;
	virtual void addChild(MoveInput mi) = 0;
	virtual void deleteCurrentChild() = 0;
	virtual GameInput getGameInput() = 0;
};