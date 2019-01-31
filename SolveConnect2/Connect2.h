#pragma once
#include <string>
#include <vector>
#include "Tile.h"
#include "Path.h"
#include "Board.h"
#include "MoveInput.h"
#include "BoardOccupy.h"


class Connect2
{
public:
	Connect2(std::string fileName);

	bool isSolved();
	int getNumOfPaths();
	bool matches(Connect2& board);
	bool moveUserInput(MoveInput mi);
	bool move(MoveInput mi);
	void undo();
	void reset();

	int getPathDisplayId(int pathId);
	std::string getDisplayStr();
	std::string getIdStr();
	std::string getIdStr2();

private:
	int getPathIdFromDisplayId(int pathDisplayId);

protected:
	Board m_board;
	BoardOccupy m_boardOccupy;
	std::vector<Path> m_path;
	std::vector<Point> m_endPointArr;
	int m_width;
	int m_height;
	int m_lastPathMoved;
	int m_lastPathMovedLength;
};