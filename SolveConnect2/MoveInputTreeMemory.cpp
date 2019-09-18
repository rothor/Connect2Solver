#include "MoveInputTreeMemory.h"


MoveInputTreeMemory::MoveInputTreeMemory()
{
	m_node = std::unique_ptr<MoveInputTreeMemoryNode>(
		new MoveInputTreeMemoryNode(
			MoveInput()
		)
	);
}

MoveInputTreeMemory::~MoveInputTreeMemory()
{

}

std::shared_ptr<NodeInterface> MoveInputTreeMemory::getStartNode()
{
	return std::shared_ptr<NodeInterface> {
		new NodeInterfaceMemory(
			std::shared_ptr<MoveInputTreeMemoryNode>(m_node),
			std::shared_ptr<MoveInputTreeMemoryNode>(0)
		)
	};
}