#include <fstream>
#include "Board.h"
#include <iostream>

Board::Board(std::string fileName)
{
	std::ifstream readFile(fileName);
	std::string temp;
	readFile >> temp >> m_width >> temp >> m_height;
	
	m_tileArr = std::shared_ptr<std::vector<std::vector<std::shared_ptr<Tile>>>>(
		new std::vector<std::vector<std::shared_ptr<Tile>>>{}
	);
	// Resize m_tile vectors
	m_tileArr->resize(m_width);
	for (int i = 0; i < m_width; i++) {
		m_tileArr->at(i).resize(m_height);
	}
	
	// Set each tile
	std::string tile;
	for (int y = m_height - 1; y >= 0; y--) {
		for (int x = 0; x < m_width; x++) {
			readFile >> tile;
			if (tile[0] == char('n')) {
				m_tileArr->at(x).at(y) = std::shared_ptr<Tile>(new TileNeutral());
			}
			else if (tile[0] == char('b')) {
				m_tileArr->at(x).at(y) = std::shared_ptr<Tile>(new TileBlock());
			}
			else if (tile[0] == char('p')) {
				std::string pos = tile.substr(1);
				int hyphen = pos.find_first_of(char('-'));
				std::string xStr = pos.substr(0, hyphen);
				std::string yStr = pos.substr(hyphen + 1);
				int xInt = std::stoi(xStr) - 1; // The '- 1' is to account that the user will input
				int yInt = std::stoi(yStr) - 1; // the x and y pos 1-indexed (instead of 0-indexed).
				m_tileArr->at(x).at(y) = std::shared_ptr<Tile>(new TilePortal(Point(xInt, yInt)));
			}
			else if (tile[0] == char('m')) {
				std::string type = tile.substr(1);
				if (type == "tl")
					m_tileArr->at(x).at(y) = std::shared_ptr<Tile>(new TileMirror(MirrorType::tl));
				else if (type == "tr")
					m_tileArr->at(x).at(y) = std::shared_ptr<Tile>(new TileMirror(MirrorType::tr));
				else if (type == "bl")
					m_tileArr->at(x).at(y) = std::shared_ptr<Tile>(new TileMirror(MirrorType::bl));
				else
					m_tileArr->at(x).at(y) = std::shared_ptr<Tile>(new TileMirror(MirrorType::br));
			}
			else if (tile[0] == char('s')) {
				std::string other = tile.substr(1);
				int hyphen = other.find_first_of(char('-'));
				std::string pathIdStr = other.substr(0, hyphen);
				int pathId = std::stoi(pathIdStr);
				m_tileArr->at(x).at(y) = std::shared_ptr<Tile>(new TileStart(pathId));
			}
			else if (tile[0] == char('e')) {
				int pathId = std::stoi(tile.substr(1));
				m_tileArr->at(x).at(y) = std::shared_ptr<Tile>(new TileEnd(pathId));
			}
		}
	}

	readFile.close();
}

MoveInstructionsExit Board::getMoveInstructionsExit(Point ptStart, PathInfoForBoard pathInfo)
{
	if (ptStart.x < 0 || ptStart.x > m_width - 1 || ptStart.y < 0 || ptStart.y > m_height - 1) // if point is out of bounds
		return MoveInstructionsExit(); // return default instructions
	else
		return MoveInstructionsExit(m_tileArr->at(ptStart.x).at(ptStart.y)->getMoveInstructionsExit(pathInfo));
}

MoveInstructionsEnter Board::getMoveInstructionsEnter(Point ptDest, PathInfoForBoard pathInfo)
{
	if (ptDest.x < 0 || ptDest.x > m_width - 1 || ptDest.y < 0 || ptDest.y > m_height - 1) // if point is out of bounds
		return MoveInstructionsEnter(); // return default instructions
	else
		return MoveInstructionsEnter(m_tileArr->at(ptDest.x).at(ptDest.y)->getMoveInstructionsEnter(pathInfo));
}

MoveInstructions Board::getMoveInstructions(Point ptStart, Point ptDest, PathInfoForBoard pathInfo)
{
	return MoveInstructions(getMoveInstructionsEnter(ptDest, pathInfo),
		getMoveInstructionsExit(ptStart, pathInfo));
}

int Board::getWidth()
{
	return m_width;
}

int Board::getHeight()
{
	return m_height;
}

// This will cause an error if wrong inputs are passed.
std::shared_ptr<Tile> Board::getTile(int x, int y)
{
	return m_tileArr->at(x).at(y);
}