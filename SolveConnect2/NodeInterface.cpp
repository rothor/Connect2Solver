#include "NodeInterface.h"


NodeInterface::NodeInterface()
{

}

bool NodeInterface::getNextChild(std::shared_ptr<NodeInterface>& node)
{
	return false;
}

void NodeInterface::addChild(MoveInput mi)
{

}

void NodeInterface::deleteCurrentChild()
{

}

GameInput NodeInterface::getGameInput()
{
	return GameInput();
}