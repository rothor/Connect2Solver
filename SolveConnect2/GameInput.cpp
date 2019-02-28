#include "GameInput.h"


GameInput::GameInput()
{

}


#include <string>
#include <sstream>
#include <list>
#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::list<std::string> split(const std::string &s, char delim) {
	std::list<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

GameInput::GameInput(std::string saveString)
{
	std::list<std::string> miStrArr = split(saveString, ' ');
	for (auto miStr : miStrArr) {
		miArr.push_back(MoveInput(miStr));
	}
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
	}
	return retStr;
}

std::string GameInput::getSaveString()
{
	std::string ret;
	for (auto iter = miArr.begin(); iter != miArr.end(); iter++) {
		ret += (*iter).getSaveStr();
		if (++iter-- != miArr.end())
			ret += " ";
	}
	return ret;
}