#include "Point.h"


Point::Point(int xIn, int yIn)
{
	x = xIn;
	y = yIn;
}

Point::~Point()
{

}

int Point::dist(Point p)
{
	int distX = x - p.x;
	if (distX < 0)
		distX *= -1;
	int distY = y - p.y;
	if (distY < 0)
		distY *= -1;
	return distX + distY;
}

bool Point::equals(Point p)
{
	return p.x == x && p.y == y;
}