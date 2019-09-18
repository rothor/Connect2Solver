#pragma once
#include "SolvedInterface.h"
#include "Connect2.h"


class SolvedInterfaceComplete : public SolvedInterface
{
public:
	bool isSolved(Connect2& game);
};