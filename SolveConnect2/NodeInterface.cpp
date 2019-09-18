#include "NodeInterface.h"


NodeInterface::NodeInterface()
{

}

std::list<std::shared_ptr<NodeInterface>> NodeInterface::getChildren()
{
	return std::list<std::shared_ptr<NodeInterface>>{};
}

void NodeInterface::addChild(MoveInput mi)
{

}

void NodeInterface::deleteThisNode()
{

}

MoveInput NodeInterface::getMoveInput()
{
	return MoveInput();
}