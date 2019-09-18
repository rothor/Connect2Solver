#pragma once
#include <memory>
#include <list>
#include <string>
#include "MoveInput.h"
#include "RecursionStruct.h"
#include "NodeInterface.h"


class Recursor
{
public:
	Recursor() = delete;
	static void addNextInputs(RecursionStruct& rs);

protected:
	static bool recurse(RecursionStruct& rs, std::shared_ptr<NodeInterface>& node, bool first = false);
	static bool addValidMoves(RecursionStruct& rs, std::shared_ptr<NodeInterface>& node);
	static bool addMove(RecursionStruct& rs, std::shared_ptr<NodeInterface>& node, MoveInput& mi);
};