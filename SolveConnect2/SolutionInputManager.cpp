#include "SolutionInputManager.h"


SolutionInputManager::SolutionInputManager(Connect2 game) :
	m_game(game),
	m_gameStart(game)
{

}

// This expects mi in the form of user input
void SolutionInputManager::addMove(MoveInput mi)
{
	if (!m_game.move(mi))
		return;

	std::string hash = m_game.getIdStr();
	bool inserted = false;
	for (auto rit = m_gi.miArr.rbegin(); rit != m_gi.miArr.rend(); rit++) {
		if ((*rit).pathId == mi.pathId) {
			m_game = m_gameStart;
			auto itPos = m_gi.miArr.begin();
			for (auto it = m_gi.miArr.begin(); it != m_gi.miArr.end(); it++) {
				m_game.move(*it);
				if (std::distance(m_gi.miArr.begin(), it) == std::distance(rit, m_gi.miArr.rend()) - 1) {
					m_game.move(mi);
					itPos = it;
				}
			}
			if (m_game.getIdStr() == hash) {
				m_gi.miArr.insert(++itPos, mi);
				inserted = true;
			}
			break;
		}
	}
	if (!inserted)
		m_gi.miArr.push_back(mi);
	m_game = m_gameStart;
	m_game.moveAll(m_gi);
	
	auto ret = m_mapIdToGi.insert(std::pair<std::string, GameInput>(m_game.getIdStr(), m_gi));
	if (!ret.second) { // if insert failed
		m_gi = ret.first->second;
	}
}

GameInput SolutionInputManager::getSolution()
{
	return m_gi;
}

Connect2 SolutionInputManager::getGameStart()
{
	return m_gameStart;
}