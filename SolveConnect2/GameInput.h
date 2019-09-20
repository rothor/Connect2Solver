#pragma once
#include <list>
#include <string>
#include "MoveInput.h"


class GameInput
{
public:
	GameInput();
	GameInput(const std::string& saveString);
	GameInput(const MoveInput& mi);

	std::string getDisplayStr();
	std::string getSaveString();
	void append(const GameInput& gi);
	void append(const MoveInput& mi);
	
public:
	std::list<MoveInput> miArr;
};
