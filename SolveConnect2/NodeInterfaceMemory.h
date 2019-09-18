#pragma once
#include "NodeInterface.h"
#include "MoveInputTreeMemoryNode.h"


class NodeInterfaceMemory : public NodeInterface
{
public:
	NodeInterfaceMemory(std::shared_ptr<MoveInputTreeMemoryNode> node, std::shared_ptr<MoveInputTreeMemoryNode> parentNode);
	~NodeInterfaceMemory();
	std::list<std::shared_ptr<NodeInterface>> getChildren();
	void addChild(MoveInput mi);
	void deleteThisNode();
	MoveInput getMoveInput();

protected:
	std::shared_ptr<MoveInputTreeMemoryNode> m_node;
	std::shared_ptr<MoveInputTreeMemoryNode> m_parentNode;
};