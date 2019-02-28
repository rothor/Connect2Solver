#pragma once
#include <list>
#include <string>
#include "MoveInput.h"


class GameInput
{
public:
	GameInput();
	GameInput(std::string saveString);

	std::string getDisplayStr();
	std::string getSaveString();
	
public:
	std::list<MoveInput> miArr;
};
