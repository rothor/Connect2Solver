#pragma once
#include <Windows.h>


class HighResolutionTimer
{
public:
	HighResolutionTimer();

private:
	LARGE_INTEGER startTime;
	LARGE_INTEGER frequency;

public:
	void reset();
	long long readMilli();
	long long readMicro();
};