#pragma once
#include <list>
#include <string>
#include "MoveInput.h"


class GameInput
{
public:
	GameInput();

	std::string getDisplayStr();
	
public:
	std::list<MoveInput> miArr;
};
