#include <fstream>
#include "Connect2.h"
#include "Direction.h"
#include <iostream>

bool staticMove(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy);
bool staticMoveDo(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy);
bool staticMoveUndo(Path& path, Board& board, BoardOccupy& boardOccupy);

Connect2::Connect2(std::string fileName) :
	m_board(fileName),
	m_boardOccupy(fileName),
	m_lastPathMoved(0)
{
	std::ifstream readFile(fileName);
	std::string temp;
	readFile >> temp >> m_width >> temp >> m_height;

	// Loop through each tile
	std::string tile;
	int totalTiles = m_width * m_height;
	for (int y = m_height - 1; y >= 0; y--) {
		for (int x = 0; x < m_width; x++) {
			readFile >> tile;
			if (tile[0] == char('s')) {
				std::string other = tile.substr(1);
				int hyphen = other.find_first_of(char('-'));
				std::string pathIdStr = other.substr(0, hyphen);
				std::string pathLengthStr = other.substr(hyphen + 1);
				int pathId = std::stoi(pathIdStr);
				int pathLength = std::stoi(pathLengthStr);
				m_path.push_back(Path(pathId, pathLength, Point(x, y)));
			}
			else if (tile[0] == char('e')) {
				m_endPointArr.push_back(Point(x, y));
			}
		}
	}
	
	readFile.close();
}

bool Connect2::isSolved()
{
	for (int i = 0; i < m_endPointArr.size(); i++) {
		if (!m_boardOccupy.isOccupied(m_endPointArr[i]))
			return false;
	}

	return true;
}

int Connect2::getNumOfPaths()
{
	return m_path.size();
}

bool Connect2::matches(Connect2& board)
{
	// Fast check first
	for (int i = 0; i < m_path.size(); i++) {
		if (m_path[i].getLength() != board.m_path[i].getLength())
			return false;
	}

	// Then slow check
	for (int i = 0; i < m_path.size(); i++) {
		for (int j = 0; j < m_path[i].getLength(); j++) {
			if (m_path[i].getMove(j).direction != board.m_path[i].getMove(j).direction)
				return false;
		}
	}

	return true;
}

bool Connect2::moveUserInput(MoveInput mi)
{
	// First, mi.pathId is converted from the display id to the actual id.
	mi.pathId = getPathIdFromDisplayId(mi.pathId);

	int length = m_path[mi.pathId].getLength();
	bool moveValid = staticMove(mi, m_path[mi.pathId], m_board, m_boardOccupy);
	if (moveValid) {
		m_lastPathMoved = mi.pathId;
		m_lastPathMovedLength = length;
	}

	return moveValid;
}

bool Connect2::move(MoveInput mi)
{
	int length = m_path[mi.pathId].getLength();
	bool moveValid = staticMove(mi, m_path[mi.pathId], m_board, m_boardOccupy);
	if (moveValid) {
		m_lastPathMoved = mi.pathId;
		m_lastPathMovedLength = length;
	}

	return moveValid;
}

/* Undoes the last move. Only keeps track of the last move,
 * so this shouldn't be called more than once between each
 * move.
 */
void Connect2::undo()
{
	while (m_path[m_lastPathMoved].getLength() > m_lastPathMovedLength) {
		PathMove pathMove = m_path[m_lastPathMoved].getLastMove();
		m_boardOccupy.setOccupied(pathMove.ptDest, false);
		m_path[m_lastPathMoved].undoLastMove();
	}
}

bool staticMove(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy)
{
	Point pt = path.getPos();
	if (!path.havePreviousMoves())
		return staticMoveDo(mi, path, board, boardOccupy);

	PathMove move = path.getLastMove();
	if ((move.direction == Direction::left && mi.direction == Direction::right) ||
		(move.direction == Direction::right && mi.direction == Direction::left) ||
		(move.direction == Direction::down && mi.direction == Direction::up) ||
		(move.direction == Direction::up && mi.direction == Direction::down)) {
		return staticMoveUndo(path, board, boardOccupy);
	}
	else
		return staticMoveDo(mi, path, board, boardOccupy);
}

bool staticMoveDo(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy)
{
	path.setDirection(mi.direction);
	bool reset = false;
	bool forced = false;
	int startLength = path.getLength();
	int lastValidLength = startLength;

	while (true) {
		// Determine if we can do move
		Point ptDest = path.getDestPoint();
		Point ptStart = path.getPos();
		MoveInstructions instr = board.getMoveInstructions(ptStart, ptDest, path.getInfoForBoard());
		if (instr.mustTeleport) {
			ptDest = instr.teleportPoint;
			instr = board.getMoveInstructions(ptStart, ptDest, path.getInfoForBoard());
		}
		if (instr.resetIfOnlyForcedMovesAfter)
			reset = true;
		forced = instr.isForced;
		if (!forced)
			reset = false;
		if (path.isFull())
			break;
		if (boardOccupy.isOccupied(ptDest))
			break;
		if (!instr.canEnter)
			break;
		
		// Set stuff pre-move
		if (!instr.isForced)
			lastValidLength = path.getLength();

		// Do move
		PathMove pathMove(ptStart, ptDest, path.getDirection(), instr.isForced, instr.mustTeleport);
		path.doMove(pathMove);
		boardOccupy.setOccupied(ptDest, true);

		// Set stuff
		if (instr.changeDirectionAfterMove)
			path.setDirection(instr.newDirection);
	}

	if (reset) {
		while (path.getLength() > startLength) {
			PathMove pathMove = path.getLastMove();
			boardOccupy.setOccupied(pathMove.ptDest, false);
			path.undoLastMove();
		}

		return false;
	}

	if (forced) {
		while (path.getLength() > lastValidLength) {
			PathMove pathMove = path.getLastMove();
			boardOccupy.setOccupied(pathMove.ptDest, false);
			path.undoLastMove();
		}
	}

	if (path.getLength() == startLength)
		return false;
	
	return true;
}

bool staticMoveUndo(Path& path, Board& board, BoardOccupy& boardOccupy)
{
	if (!path.havePreviousMoves())
		return false;

	PathMove lastMove = path.getLastMove();
	Direction prevDirection = lastMove.direction;
	bool prevWasForced = false;

	while (path.havePreviousMoves()) {
		lastMove = path.getLastMove();

		bool doUndo = lastMove.isForced || prevWasForced || lastMove.direction == prevDirection;
		if (!doUndo)
			return true;

		prevWasForced = lastMove.isForced;
		prevDirection = lastMove.direction;

		if (doUndo) {
			boardOccupy.setOccupied(lastMove.ptDest, false);
			path.undoLastMove();
		}
	}
}

void Connect2::reset()
{
	// Reset paths
	for (int i = 0; i < m_path.size(); i++) {
		m_path[i].reset();
	}
	
	// Reset m_boardOccupy
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			m_boardOccupy.setOccupied(Point(x, y), false);
		}
	}
}


int Connect2::getPathDisplayId(int pathId)
{
	return m_path[pathId].getId();
}

// Returns string for displaying in the console output.
std::string Connect2::getDisplayStr()
{
	std::vector<std::vector<std::string>> board;
	board.resize(m_width);
	for (int i = 0; i < m_width; i++) {
		board[i].resize(m_height, "- ");
	}

	for (int i = 0; i < m_path.size(); i++) {
		int displayId = m_path[i].getId();
		Point pt = m_path[i].getStartPoint();
		board[pt.x][pt.y] = std::to_string(displayId) + " ";
		for (int j = 0; j < m_path[i].getLength(); j++) {
			PathMove move = m_path[i].getMove(j);
			board[move.ptDest.x][move.ptDest.y] = std::to_string(displayId) + " ";
		}
	}

	std::string ret;
	for (int y = m_height - 1; y >= 0; y--) {
		for (int x = 0; x < m_width; x++) {
			ret += board[x][y];
		}
		ret += "\n";
	}

	return ret;
}

// This is the old version.
std::string Connect2::getIdStr()
{
	return getIdStr2();

	std::vector<std::vector<std::string>> board;
	board.resize(m_width);
	for (int i = 0; i < m_width; i++) {
		board[i].resize(m_height, "-");
	}

	for (int i = 0; i < m_path.size(); i++) {
		Point pt = m_path[i].getStartPoint();
		board[pt.x][pt.y] = std::to_string(getPathDisplayId(i));
		for (int j = 0; j < m_path[i].getLength(); j++) {
			PathMove move = m_path[i].getMove(j);
			board[move.ptDest.x][move.ptDest.y] = std::to_string(i);
		}
	}

	std::string ret;
	for (int y = m_height - 1; y >= 0; y--) {
		for (int x = 0; x < m_width; x++) {
			ret += board[x][y];
		}
	}

	return ret;
}

// In general, this will give much shorter names than getMoveArrStr(), which is why
// this is preferred.
std::string Connect2::getIdStr2()
{
	std::string ret;
	ret += m_path[0].getMoveArrStr();
	for (int i = 1; i < m_path.size(); i++) {
		ret += "-" + m_path[i].getMoveArrStr();
	}
	return ret;
}

/* Since this method returns 0 by default, it could lead to unexected behavior
 * when the user inputs a wrong path id (it will operate on m_path[0] then).
 */
int Connect2::getPathIdFromDisplayId(int pathDisplayId)
{
	for (int i = 0; i < m_path.size(); i++) {
		if (m_path[i].getId() == pathDisplayId)
			return i;
	}

	return 0;
}