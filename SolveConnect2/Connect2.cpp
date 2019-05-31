#include <fstream>
#include "Connect2.h"
#include "Direction.h"
#include <iostream>
#include <algorithm>

bool staticMove(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy, bool& moveWasUndo, bool singleStep = false);
bool staticMoveDo(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy, bool singleStep = false);
bool staticMoveUndo(Path& path, Board& board, BoardOccupy& boardOccupy, bool singleStep = false);


bool Connect2::sortPathLength(int a, int b)
{
	return m_path[a].getMaxLength() < m_path[b].getMaxLength();
}

#include <functional>

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

	// Get max path id
	int maxId = 0;
	for (int y = m_height - 1; y >= 0; y--) {
		for (int x = 0; x < m_width; x++) {
			readFile >> tile;
			if (tile[0] == char('s')) {
				std::string other = tile.substr(1);
				int hyphen = other.find_first_of(char('-'));
				std::string pathIdStr = other.substr(0, hyphen);
				int pathId = std::stoi(pathIdStr);
				if (pathId > maxId)
					maxId = pathId;
			}
		}
	}
	for (int i = 0; i < maxId; i++) {
		m_path.push_back(Path(0, 0, Point(0, 0)));
		m_endPointArr.push_back(Point(0, 0));
	}
	readFile.close();

	std::ifstream readFile2(fileName);
	readFile2 >> temp >> temp >> temp >> temp;

	// Get path data
	for (int y = m_height - 1; y >= 0; y--) {
		for (int x = 0; x < m_width; x++) {
			readFile2 >> tile;
			if (tile[0] == char('s')) {
				std::string other = tile.substr(1);
				int hyphen = other.find_first_of(char('-'));
				std::string pathIdStr = other.substr(0, hyphen);
				std::string pathLengthStr = other.substr(hyphen + 1);
				int pathId = std::stoi(pathIdStr);
				int pathLength = std::stoi(pathLengthStr);
				m_path[pathId - 1] = Path(pathId, pathLength, Point(x, y));
			}
			else if (tile[0] == char('e')) {
				std::string pathIdStr = tile.substr(1);
				int pathId = std::stoi(pathIdStr);
				m_endPointArr[pathId - 1] = Point(x, y);
			}
		}
	}

	readFile2.close();

	
	for (int i = 0; i < m_path.size(); i++) {
		m_pathLengthVec.push_back(i);
	}
	std::sort(m_pathLengthVec.begin(), m_pathLengthVec.end(), std::bind(&Connect2::sortPathLength, this, std::placeholders::_1, std::placeholders::_2));
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

bool Connect2::move(MoveInput& mi)
{
	Path& path = getPath(mi.pathId);
	int length = path.getLength();
	bool moveValid = staticMove(mi, path, m_board, m_boardOccupy, m_lastMoveWasUndo, mi.singleStep);
	if (moveValid) {
		m_lastPathMoved = mi.pathId;
		m_lastPathMovedLength = length;
	}

	return moveValid;
}

bool Connect2::moveOnlyDo(MoveInput& mi)
{
	Path& path = getPath(mi.pathId);
	int length = path.getLength();
	bool moveValid = staticMove(mi, path, m_board, m_boardOccupy, m_lastMoveWasUndo, mi.singleStep);
	if (moveValid) {
		m_lastPathMoved = mi.pathId;
		m_lastPathMovedLength = length;
	}
	if (m_lastMoveWasUndo) {
		undo();
		return false;
	}

	return moveValid;
}

void Connect2::moveAll(GameInput& gi)
{
	for (MoveInput& mi : gi.miArr) {
		move(mi);
	}
}

void staticMoveDoForUndo(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy, int length);
/* Undoes the last move. Only keeps track of the last move,
 * so this shouldn't be called more than once between each
 * move.
 */
void Connect2::undo()
{
	Path& path = getPath(m_lastPathMoved);
	if (m_lastMoveWasUndo) {
		MoveInput mi(path.getId(), path.getLastMoveDirection());
		staticMoveDoForUndo(mi, path, m_board, m_boardOccupy, m_lastPathMovedLength);
	}
	
	while (path.getLength() > m_lastPathMovedLength) {
		PathMove pathMove = path.getLastMove();
		m_boardOccupy.setOccupied(pathMove.ptDest, false);
		path.undoLastMove();
	}
}
void staticMoveDoForUndo(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy, int length)
{
	Direction newDirection = mi.direction;
	bool reset = false;
	bool forced = false;
	int startLength = path.getLength();
	int lastValidLength = startLength;

	while (true) {
		// Determine if we can do move
		if (path.getLength() == length)
			break;

		path.setNewDirection(newDirection);
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
		if (path.isFull() || boardOccupy.isOccupied(ptDest) || !instr.canEnter)
			break;

		// Set stuff pre-move
		if (!forced)
			lastValidLength = path.getLength();

		// Do move
		PathMove pathMove(ptStart, ptDest, newDirection, forced, instr.mustTeleport);
		path.doMove(pathMove);
		boardOccupy.setOccupied(ptDest, true);

		// Set stuff
		if (instr.changeDirectionAfterMove)
			newDirection = instr.newDirection;
	}
}

bool staticMove(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy, bool& moveWasUndo, bool singleStep)
{
	Point pt = path.getPos();
	if (!path.havePreviousMoves()) {
		moveWasUndo = false;
		return staticMoveDo(mi, path, board, boardOccupy, singleStep);
	}

	PathMove move = path.getLastMove();
	if ((move.direction == Direction::left && mi.direction == Direction::right) ||
		(move.direction == Direction::right && mi.direction == Direction::left) ||
		(move.direction == Direction::down && mi.direction == Direction::up) ||
		(move.direction == Direction::up && mi.direction == Direction::down)) {
		moveWasUndo = true;
		return staticMoveUndo(path, board, boardOccupy, singleStep);
	}
	else {
		moveWasUndo = false;
		return staticMoveDo(mi, path, board, boardOccupy, singleStep);
	}
}

bool staticMoveDo(MoveInput& mi, Path& path, Board& board, BoardOccupy& boardOccupy, bool singleStep)
{
	Direction newDirection = mi.direction;
	bool reset = false;
	bool forced = false;
	int startLength = path.getLength();
	int lastValidLength = startLength;

	while (true) {
		// Determine if we can do move
		path.setNewDirection(newDirection);
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
		if (path.isFull() || boardOccupy.isOccupied(ptDest) || !instr.canEnter)
			break;
		
		// Set stuff pre-move
		if (!forced)
			lastValidLength = path.getLength();

		// Do move
		PathMove pathMove(ptStart, ptDest, newDirection, forced, instr.mustTeleport);
		path.doMove(pathMove);
		boardOccupy.setOccupied(ptDest, true);

		// Set stuff
		if (instr.changeDirectionAfterMove)
			newDirection = instr.newDirection;
		
		if (singleStep && instr.canStop) {
			reset = false;
			break;
		}
	}

	if (reset) {
		while (path.getLength() > startLength) {
			PathMove pathMove = path.getLastMove();
			boardOccupy.setOccupied(pathMove.ptDest, false);
			path.undoLastMove();
		}
	}
	else if (forced && !singleStep) {
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

bool staticMoveUndo(Path& path, Board& board, BoardOccupy& boardOccupy, bool singleStep)
{
	if (!path.havePreviousMoves())
		return false;

	PathMove lastMove = path.getLastMove();
	Direction prevDirection = lastMove.direction;
	bool prevWasForced = false;

	while (path.havePreviousMoves()) {
		lastMove = path.getLastMove();

		bool doUndo = prevWasForced || lastMove.direction == prevDirection;
		if (!doUndo)
			return true;

		prevWasForced = lastMove.isForced;
		prevDirection = lastMove.direction;

		if (doUndo) {
			boardOccupy.setOccupied(lastMove.ptDest, false);
			path.undoLastMove();
			if (singleStep && !prevWasForced) // ???
				return true; // ???
		}
	}
	return true;
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

bool Connect2::pathIsSolved(int pathId)
{
	return m_boardOccupy.isOccupied(m_endPointArr[pathId]);
}

bool Connect2::pathIsFull(int pathId)
{
	return m_path[pathId].isFull();
}

bool Connect2::pathCanBeSolved(int pathId)
{
	Point pos = m_path[pathId].getPos();
	int dist = abs(pos.x - m_endPointArr[pathId].x) + abs(pos.y - m_endPointArr[pathId].y);
	return m_path[pathId].getMaxLength() - m_path[pathId].getLength() >= dist;
}

std::vector<int>* Connect2::getPathIdsOrderedByLength()
{
	return &m_pathLengthVec;
}

int Connect2::getPathDisplayId(int pathId)
{
	return m_path[pathId].getId();
}

int min(int x, int y)
{
	return x < y ? x : y;
}

// Returns string for displaying in the console output.
std::string Connect2::getDisplayStr()
{
	std::string unnoccupiedTileStr = " ' ";
	std::string occupiedTileStr = " o ";
	std::string blockTileStr = "[x]";
	std::string portalOccupiedStr = "(o)";
	std::string portalUnnoccupiedStr = "( )";
	std::string mirrorTileTlStr = " \/)";
	std::string mirrorTileTrStr = "(\\ ";
	std::string mirrorTileBlStr = " \\)";
	std::string mirrorTileBrStr = "(\/ ";
	std::string startTileStr = "#?#";
	std::string endTileUnnoccupiedStr = "-?-";
	std::string endTileOccupiedStr = "{?}";
	std::string dirUp = " A ";
	std::string dirDown = " V ";
	std::string dirLeft = " < ";
	std::string dirRight = " > ";
	std::string placeholderStr = "???";

	// Board vec
	std::vector<std::vector<std::string>> board;
	board.resize(m_width);
	for (int i = 0; i < m_width; i++) {
		board[i].resize(m_height, unnoccupiedTileStr);
	}
	// Horizontal connections
	std::vector<std::vector<std::string>> connHori;
	connHori.resize(m_width - 1);
	for (int i = 0; i < m_width - 1; i++) {
		connHori[i].resize(m_height, "  ");
	}
	// Vertical connections
	std::vector<std::vector<std::string>> connVert;
	connVert.resize(m_width);
	for (int i = 0; i < m_width; i++) {
		connVert[i].resize(m_height - 1, " ");
	}

	// Loop through each path
	for (int i = 0; i < m_path.size(); i++) {
		// Loop through each path move
		for (int j = 0; j < m_path[i].getLength(); j++) {
			PathMove move = m_path[i].getMove(j);

			// Set connection
			if (!move.isTeleport) {
				if (move.ptDest.x != move.ptBegin.x)
					connHori[min(move.ptBegin.x, move.ptDest.x)][move.ptBegin.y] = "--";
				else // else the y's must be different
					connVert[move.ptBegin.x][min(move.ptBegin.y, move.ptDest.y)] = "|";
			}
		}
	}
	// Loop through each tile
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			std::shared_ptr<Tile> tile = m_board.getTile(x, y);
			if (tile->m_type == TileType::neutral) {
				if (m_boardOccupy.isOccupied(Point(x, y)))
					board[x][y] = occupiedTileStr;
				else
					; // board[x][y] = unnoccupiedTileStr;
			}
			else if (tile->m_type == TileType::block)
				board[x][y] = blockTileStr;
			else if (tile->m_type == TileType::portal) {
				if (m_boardOccupy.isOccupied(Point(x, y)))
					board[x][y] = portalOccupiedStr;
				else
					board[x][y] = portalUnnoccupiedStr;
			}
			else if (tile->m_type == TileType::mirror) {
				Tile* tileNone = &*tile; 
				TileMirror* mirror = static_cast<TileMirror*>(tileNone);
				if (mirror->m_mirrorType == MirrorType::bl)
					board[x][y] = mirrorTileBlStr;
				else if (mirror->m_mirrorType == MirrorType::br)
					board[x][y] = mirrorTileBrStr;
				else if (mirror->m_mirrorType == MirrorType::tl)
					board[x][y] = mirrorTileTlStr;
				else
					board[x][y] = mirrorTileTrStr;
			}
			else if (tile->m_type == TileType::start) {
				Tile* tileNone = &*tile;
				TileStart* start = static_cast<TileStart*>(tileNone);
				board[x][y] = startTileStr;
				int pos = board[x][y].find("?");
				board[x][y].replace(pos, 1, std::to_string(start->m_pathId));
			}
		}
	}
	// Loop through each path
	for (int i = 0; i < m_path.size(); i++) {
		// Set path arrow
		if (m_path[i].getLength() > 0) {
			std::string dirStr;
			Direction direction = m_path[i].getDirection();
			if (direction == Direction::up)
				dirStr = dirUp;
			else if (direction == Direction::down)
				dirStr = dirDown;
			else if (direction == Direction::left)
				dirStr = dirLeft;
			else
				dirStr = dirRight;
			board[m_path[i].getPos().x][m_path[i].getPos().y] = dirStr;
		}
	}
	// Loop through each tile
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			std::shared_ptr<Tile> tile = m_board.getTile(x, y);
			if (tile->m_type == TileType::end) {
				Tile* tileNone = &*tile;
				TileEnd* start = static_cast<TileEnd*>(tileNone);
				if (m_boardOccupy.isOccupied(Point(x, y)))
					board[x][y] = endTileOccupiedStr;
				else
					board[x][y] = endTileUnnoccupiedStr;
				int pos = board[x][y].find("?");
				board[x][y].replace(pos, 1, std::to_string(start->m_pathId));
			}
		}
	}

	// Start composing return string
	std::string ret;
	ret += " ";
	for (int x = 0; x < m_width * 5 - 0; x++) {
		ret += "-"; // top border
	}
	ret += "\n";

	for (int y = m_height - 1; y >= 0; y--) {
		ret += "| "; // left border
		for (int x = 0; x < m_width; x++) {
			ret += board[x][y];
			if (x < m_width - 1)
				ret += connHori[x][y];
		}
		ret += " |"; // right border
		ret += "\n";

		if (y > 0) {
			ret += "|  "; // left border
			for (int x = 0; x < m_width; x++) {
				ret += connVert[x][y - 1];
				if (x < m_width - 1)
					ret += "    ";
			}
			ret += "  |"; // right border
			ret += "\n";
		}
	}

	ret += " ";
	for (int x = 0; x < m_width * 5 - 0; x++) {
		ret += "-"; // bottom border
	}
	ret += "\n";

	// Loop through each path
	for (int i = 0; i < m_path.size(); i++) {
		ret += std::to_string(m_path[i].getId()) + " = " +
			std::to_string(m_path[i].getLength()) + "\/" +
			std::to_string(m_path[i].getMaxLength());
		if (i < m_path.size() - 1)
			ret += " , ";
	}

	return ret;
}

std::string Connect2::getIdStr()
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
	return pathDisplayId - 1;

	//return 0;
}

Path& Connect2::getPath(int pathDisplayId)
{
	return m_path[getPathIdFromDisplayId(pathDisplayId)];
}