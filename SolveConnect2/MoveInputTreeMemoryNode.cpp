#include "MoveInputTreeMemoryNode.h"


MoveInputTreeMemoryNode::MoveInputTreeMemoryNode(MoveInput mi) :
	m_mi(mi)
{

}

void MoveInputTreeMemoryNode::addChild(MoveInput mi)
{
	m_childNodes.push_back(
		std::shared_ptr<MoveInputTreeMemoryNode>{
			new MoveInputTreeMemoryNode(mi)
		}
	);
}