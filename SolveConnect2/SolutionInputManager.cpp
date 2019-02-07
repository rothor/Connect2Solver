#include "SolutionInputManager.h"


SolutionInputManager::SolutionInputManager(Connect2 game) :
	m_game(game)
{

}

// This expects mi in the form of user input
void SolutionInputManager::addMove(MoveInput mi)
{
	m_gi.miArr.push_back(mi);
	m_game.move(mi);
	auto ret = m_mapIdToGi.insert(std::pair<std::string, GameInput>(m_game.getIdStr(), m_gi));
	if (!ret.second) { // if insert failed
		m_gi = ret.first->second;
	}
}

GameInput SolutionInputManager::getSolution()
{
	return m_gi;
}

void SolutionInputManager::reset()
{
	m_game.reset();
	m_gi.miArr.clear();
	m_mapIdToGi.clear();
}