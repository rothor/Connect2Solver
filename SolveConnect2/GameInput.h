#pragma once
#include <list>
#include <string>
#include "MoveInput.h"


class GameInput
{
public:
	GameInput();
	
public:
	std::list<MoveInput> miArr;
};

