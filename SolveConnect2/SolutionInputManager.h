#pragma once
#include <map>
#include <string>
#include "IdManager.h"
#include "GameInput.h"
#include "MoveInput.h"
#include "Connect2.h"


class SolutionInputManager
{
public:
	SolutionInputManager(Connect2 game);

	void addMove(MoveInput mi);
	GameInput getSolution();
	void reset();
	
protected:
	Connect2 m_game;
	GameInput m_gi;
	std::map<std::string, GameInput> m_mapIdToGi;
};

