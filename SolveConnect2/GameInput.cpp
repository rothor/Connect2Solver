#include "GameInput.h"


GameInput::GameInput()
{

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