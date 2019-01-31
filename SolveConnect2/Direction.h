#pragma once


enum class Direction { left, down, right, up };

char directionToChar(Direction dir);
Direction charToDirection(char charIn);
