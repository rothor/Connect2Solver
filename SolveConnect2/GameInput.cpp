#include "GameInput.h"
#include "Globals.h"
#include "Misc.h"


GameInput::GameInput()
{

}

GameInput::GameInput(const std::string& saveString)
{
	std::list<std::string> miStrArr = Misc::split(saveString, ' ');
	for (auto miStr : miStrArr) {
		miArr.push_back(MoveInput(miStr));
	}
}

GameInput::GameInput(const MoveInput& mi)
{
	miArr.push_back(mi);
}

std::string GameInput::getDisplayStr()
{
	std::string retStr;
	int prevPathId = -1;
	bool skippedFirstLinebreak = false;
	for (const MoveInput& mi : miArr) {
		if (mi.pathId != prevPathId) {
			if (skippedFirstLinebreak)
				retStr += "\n";
			else
				skippedFirstLinebreak = true;
			retStr += std::to_string(mi.pathId) + ": ";
		}
		prevPathId = mi.pathId;
		retStr += directionToChar(mi.direction) + 'A' - 'a'; // to upper
		if (mi.singleStep)
			retStr += Globals::singleMoveChar;
	}
	return retStr;
}

std::string GameInput::getSaveString()
{
	std::string ret;
	bool first = true;
	for (auto iter = miArr.begin(); iter != miArr.end(); iter++) {
		if (first)
			first = false;
		else
			ret += " ";
		ret += (*iter).getSaveStr();
	}
	return ret;
}

void GameInput::append(const GameInput& gi)
{
	for (auto it = gi.miArr.begin(); it != gi.miArr.end(); ++it) {
		miArr.push_back(*it);
	}
}

void GameInput::append(const MoveInput& mi)
{
	miArr.push_back(mi);
}