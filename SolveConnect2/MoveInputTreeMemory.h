#pragma once
#include "MoveInputTree.h"
#include "NodeInterfaceMemory.h"
#include "MoveInputTreeMemoryNode.h"
#include <memory>


class MoveInputTreeMemory : public MoveInputTree
{
public:
	MoveInputTreeMemory();
	~MoveInputTreeMemory();
	std::shared_ptr<NodeInterface> getStartNode();

protected:
	std::shared_ptr<MoveInputTreeMemoryNode> m_node;
};