#pragma once
#include "NodeInterface.h"
#include <memory>


class MoveInputTree
{
public:
	virtual ~MoveInputTree() = 0;
	virtual std::shared_ptr<NodeInterface> getStartNode() = 0;
};