#pragma once
#include <map>
#include <string>
#include "GameInput.h"
#include "MoveInput.h"
#include "Connect2.h"


class SolutionInputManager
{
public:
	SolutionInputManager(Connect2 game);

	void addMove(MoveInput mi);
	GameInput getSolution();
	Connect2 getGameStart();
	
protected:
	Connect2 m_game;
	Connect2 m_gameStart;
	GameInput m_gi;
	std::map<std::string, GameInput> m_mapIdToGi;
};

