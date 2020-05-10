#pragma once


class Point
{
public:
	Point(int xIn, int yIn);
	~Point();
	int dist(Point p);
	bool equals(Point p);

public:
	int x;
	int y;
};

