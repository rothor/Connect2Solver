#include "NodeInterfaceMemory.h"


NodeInterfaceMemory::NodeInterfaceMemory(std::shared_ptr<MoveInputTreeMemoryNode> node, std::shared_ptr<MoveInputTreeMemoryNode> parentNode) :
	m_node(node),
	m_parentNode(parentNode),
	m_itInitialized(false),
	m_deleted(false)
{

}

NodeInterfaceMemory::~NodeInterfaceMemory()
{

}

bool NodeInterfaceMemory::getNextChild(std::shared_ptr<NodeInterface>& node)
{
	if (m_itInitialized) {
		if (!m_deleted)
			++m_it;
	}
	else {
		m_it = m_node->m_childNodes.begin();
		m_itInitialized = true;
	}
	m_deleted = false;

	if (m_it == m_node->m_childNodes.end())
		return false;
	else {
		node = std::shared_ptr<NodeInterface>{
			new NodeInterfaceMemory(*m_it, m_node)
		};
		return true;
	}
}

void NodeInterfaceMemory::addChild(MoveInput mi)
{
	m_node->addChild(mi);
}

void NodeInterfaceMemory::deleteCurrentChild()
{
	m_it = m_node->m_childNodes.erase(m_it);
	m_deleted = true;
}

GameInput NodeInterfaceMemory::getGameInput()
{
	return m_node->m_mi;
}