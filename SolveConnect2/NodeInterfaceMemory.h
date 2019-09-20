#pragma once
#include "NodeInterface.h"
#include "MoveInputTreeMemoryNode.h"
#include "GameInput.h"
#include <list>
#include <memory>


class NodeInterfaceMemory : public NodeInterface
{
public:
	NodeInterfaceMemory(std::shared_ptr<MoveInputTreeMemoryNode> node, std::shared_ptr<MoveInputTreeMemoryNode> parentNode);
	~NodeInterfaceMemory();
	bool getNextChild(std::shared_ptr<NodeInterface>& node);
	void addChild(MoveInput mi);
	void deleteCurrentChild();
	GameInput getGameInput();

protected:
	std::shared_ptr<MoveInputTreeMemoryNode> m_node;
	std::shared_ptr<MoveInputTreeMemoryNode> m_parentNode;
	std::list<std::shared_ptr<MoveInputTreeMemoryNode>>::iterator m_it;
	bool m_itInitialized;
	bool m_deleted;
};