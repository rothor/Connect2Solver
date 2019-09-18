#include "NodeInterfaceMemory.h"


NodeInterfaceMemory::NodeInterfaceMemory(std::shared_ptr<MoveInputTreeMemoryNode> node, std::shared_ptr<MoveInputTreeMemoryNode> parentNode) :
	m_node(node),
	m_parentNode(parentNode)
{

}

NodeInterfaceMemory::~NodeInterfaceMemory()
{

}

std::list<std::shared_ptr<NodeInterface>> NodeInterfaceMemory::getChildren()
{
	std::list<std::shared_ptr<NodeInterface>> ret;
	for (auto it = m_node->m_childNodes.begin(); it != m_node->m_childNodes.end(); it++) {
		ret.push_back(
			std::shared_ptr<NodeInterface>(
				new NodeInterfaceMemory(*it, m_node)
			)
		);
	}
	return ret;
}

void NodeInterfaceMemory::addChild(MoveInput mi)
{
	m_node->addChild(mi);
}

void NodeInterfaceMemory::deleteThisNode()
{
	for (auto it = m_parentNode->m_childNodes.begin(); it != m_parentNode->m_childNodes.end(); it++) {
		if ((*it)->m_mi.pathId == m_node->m_mi.pathId &&
			(*it)->m_mi.direction == m_node->m_mi.direction &&
			(*it)->m_mi.singleStep == m_node->m_mi.singleStep) {
			m_parentNode->m_childNodes.erase(it);
			return;
		}
	}
}

MoveInput NodeInterfaceMemory::getMoveInput()
{
	return m_node->m_mi;
}