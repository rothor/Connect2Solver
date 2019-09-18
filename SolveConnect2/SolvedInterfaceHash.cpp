#include "SolvedInterfaceHash.h"


SolvedInterfaceHash::SolvedInterfaceHash(std::string endHash) :
	m_endHash(endHash)
{

}

bool SolvedInterfaceHash::isSolved(Connect2& game)
{
	return game.getIdStr() == m_endHash;
}