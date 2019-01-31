#include <fstream>
#include "Board.h"
#include <iostream>

Board::Board(std::string fileName)
{
	std::ifstream readFile(fileName);
	std::string temp;
	readFile >> temp >> m_width >> temp >> m_height;

	// Resize m_tile vectors
	m_tileArr.resize(m_width);
	for (int i = 0; i < m_width; i++) {
		m_tileArr[i].resize(m_height);
	}
	
	// Set each tile
	std::string tile;
	for (int y = m_height - 1; y >= 0; y--) {
		for (int x = 0; x < m_width; x++) {
			readFile >> tile;
			if (tile[0] == char('n')) {
				m_tileArr[x][y] = new TileNeutral();
				//m_tileArr[x][y] = std::move(std::unique_ptr<TileNeutral>(new TileNeutral()));
			}
			else if (tile[0] == char('b')) {
				m_tileArr[x][y] = new TileBlock();
				//m_tileArr[x][y] = std::move(std::unique_ptr<TileBlock>(new TileBlock()));
			}
			else if (tile[0] == char('p')) {
				std::string pos = tile.substr(1);
				int hyphen = pos.find_first_of(char('-'));
				std::string xStr = pos.substr(0, hyphen);
				std::string yStr = pos.substr(hyphen + 1);
				int xInt = std::stoi(xStr);
				int yInt = std::stoi(yStr);
				m_tileArr[x][y] = new TilePortal(Point(xInt, yInt));
				//m_tileArr[x][y] = std::move(std::unique_ptr<TilePortal>(new TilePortal(Point(xInt, yInt))));
			}
			else if (tile[0] == char('m')) {
				std::string type = tile.substr(1);
				if (type == "tl")
					m_tileArr[x][y] = new TileMirror(MirrorType::tl);
				//m_tileArr[x][y] = std::move(std::unique_ptr<TileMirror>(new TileMirror(MirrorType::tl)));
				else if (type == "tr")
					m_tileArr[x][y] = new TileMirror(MirrorType::tr);
				//m_tileArr[x][y] = std::move(std::unique_ptr<TileMirror>(new TileMirror(MirrorType::tr)));
				else if (type == "bl")
					m_tileArr[x][y] = new TileMirror(MirrorType::bl);
				//m_tileArr[x][y] = std::move(std::unique_ptr<TileMirror>(new TileMirror(MirrorType::bl)));
				else
					m_tileArr[x][y] = new TileMirror(MirrorType::br);
				//m_tileArr[x][y] = std::move(std::unique_ptr<TileMirror>(new TileMirror(MirrorType::br)));
			}
			else if (tile[0] == char('s')) {
				std::string other = tile.substr(1);
				int hyphen = other.find_first_of(char('-'));
				std::string pathIdStr = other.substr(0, hyphen);
				int pathId = std::stoi(pathIdStr);
				m_tileArr[x][y] = new TileStart(pathId);
				//m_tileArr[x][y] = std::move(std::unique_ptr<TileStart>(new TileStart(pathId)));
			}
			else if (tile[0] == char('e')) {
				int pathId = std::stoi(tile.substr(1));
				m_tileArr[x][y] = new TileEnd(pathId);
				//m_tileArr[x][y] = std::move(std::unique_ptr<TileEnd>(new TileEnd(pathId)));
			}
		}
	}

	readFile.close();
}

Board::~Board()
{
	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			//delete m_tileArr[x][y];
		}
	}
}

MoveInstructionsExit Board::getMoveInstructionsExit(Point ptStart, PathInfoForBoard pathInfo)
{
	if (ptStart.x < 0 || ptStart.x > m_width - 1 || ptStart.y < 0 || ptStart.y > m_height - 1) // if point is out of bounds
		return MoveInstructionsExit(); // return default instructions
	else
		return MoveInstructionsExit(m_tileArr[ptStart.x][ptStart.y]->getMoveInstructionsExit(pathInfo));
}

MoveInstructionsEnter Board::getMoveInstructionsEnter(Point ptDest, PathInfoForBoard pathInfo)
{
	if (ptDest.x < 0 || ptDest.x > m_width - 1 || ptDest.y < 0 || ptDest.y > m_height - 1) // if point is out of bounds
		return MoveInstructionsEnter(); // return default instructions
	else
		return MoveInstructionsEnter(m_tileArr[ptDest.x][ptDest.y]->getMoveInstructionsEnter(pathInfo));
}

MoveInstructions Board::getMoveInstructions(Point ptStart, Point ptDest, PathInfoForBoard pathInfo)
{
	return MoveInstructions(getMoveInstructionsEnter(ptDest, pathInfo),
		getMoveInstructionsExit(ptStart, pathInfo));
}
