#pragma once


class TileTypeCounter
{
public:
	TileTypeCounter();

	int blocked;
	int open;
	int portal;
	int start;
	int end;
	int possiblePeninsula;
};