#include <fstream>
#include "BoardOccupy.h"


BoardOccupy::BoardOccupy(std::string fileName)
{
	std::ifstream readFile(fileName);
	std::string temp;
	readFile >> temp >> m_width >> temp >> m_height;

	// Resize m_tile vectors
	m_occupyArr.resize(m_width);
	for (int i = 0; i < m_width; i++) {
		m_occupyArr[i].resize(m_height, false);
	}

	readFile.close();
}

bool BoardOccupy::isOccupied(Point pt)
{
	if (pt.x < 0 || pt.x > m_width - 1 || pt.y < 0 || pt.y > m_height - 1)
		return true; // points that are out-of-bounds will return true
	
	return m_occupyArr[pt.x][pt.y];
}

void BoardOccupy::setOccupied(Point pt, bool occupied)
{
	if (pt.x < 0 || pt.x > m_width - 1 || pt.y < 0 || pt.y > m_height - 1)
		return; // points that are out-of-bounds will return true
	
	m_occupyArr[pt.x][pt.y] = occupied;
}