#pragma once
#include <string>
#include <vector>
#include "Tile.h"
#include "Path.h"
#include "Board.h"
#include "MoveInput.h"
#include "BoardOccupy.h"
#include "GameInput.h"
#include "TileTypeCounter.h"


class Connect2
{
public:
	Connect2(std::string fileName);

	bool isSolved();
	int getNumOfPaths();
	bool move(MoveInput& mi);
	bool moveOnlyDo(MoveInput& mi);
	void moveAll(GameInput& gi);
	void undo();
	void reset();
	bool pathIsSolved(int pathId);
	bool pathIsFull(int pathId);
	bool pathCanBeSolvedSlow(int pathId);
	void analyzeTile(TileTypeCounter& c, Point p);
	TileTypeCounter analyzeSurroundingTiles(Point p);
	bool checkPeninsula(std::list<Point> v);
	bool checkIsland();
	std::vector<int>* getPathIdsOrderedByLength();
	bool sortPathLength(int a, int b);

	int getPathDisplayId(int pathId);
	int getPathIdFromDisplayId(int pathDisplayId);
	void getDisplayStr();
	std::string getIdStr();
	int getPathLength(int pathDisplayId);
	std::list<Point> getLastPathPositions(int pathDisplayId, int num);

protected:
	Path& getPath(int pathDisplayId);

protected:
	Board m_board;
	BoardOccupy m_boardOccupy;
	std::vector<Path> m_path;
	std::vector<Point> m_endPointArr;
	int m_width;
	int m_height;
	std::vector<int> m_pathLengthVec;
	std::vector<std::vector<Point>> m_portalPairArr;

	int m_lastPathMoved;
	int m_lastPathMovedLength;
	bool m_lastMoveWasUndo;
	std::vector<Direction> m_lastMoveDir;
};
