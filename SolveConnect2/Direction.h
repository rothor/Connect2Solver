#pragma once


enum class Direction { left, down, right, up };

char directionToChar(Direction dir);

/* Since charToDirection returns Direction::down by default if none match,
 * this can cause unexpected behavior if a wrong input is passed to this function.
 */
Direction charToDirection(char charIn);
