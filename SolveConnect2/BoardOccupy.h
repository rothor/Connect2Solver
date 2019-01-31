#pragma once
#include <vector>
#include <string>
#include "Point.h"


class BoardOccupy
{
public:
	BoardOccupy(std::string fileName);
	bool isOccupied(Point pt);
	void setOccupied(Point pt, bool occupied);
	
protected:
	std::vector<std::vector<bool>> m_occupyArr;
	int m_width;
	int m_height;
};
