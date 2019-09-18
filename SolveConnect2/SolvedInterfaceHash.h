#pragma once
#include "SolvedInterface.h"
#include "Connect2.h"
#include <string>


class SolvedInterfaceHash : public SolvedInterface
{
public:
	SolvedInterfaceHash(std::string endHash);
	bool isSolved(Connect2& game);

protected:
	std::string m_endHash;
};