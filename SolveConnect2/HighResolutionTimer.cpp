#include "HighResolutionTimer.h"


HighResolutionTimer::HighResolutionTimer()
{
	QueryPerformanceFrequency(&frequency);
	reset();
}

void HighResolutionTimer::reset()
{
	QueryPerformanceCounter(&startTime);
}

long long HighResolutionTimer::readMilli()
{
	LARGE_INTEGER endTime;
	QueryPerformanceCounter(&endTime);
	LARGE_INTEGER elapsedMicros;
	elapsedMicros.QuadPart = endTime.QuadPart - startTime.QuadPart;
	elapsedMicros.QuadPart *= 1000000;
	elapsedMicros.QuadPart /= frequency.QuadPart;

	return elapsedMicros.QuadPart / 1000;
}

long long HighResolutionTimer::readMicro()
{
	LARGE_INTEGER endTime;
	QueryPerformanceCounter(&endTime);
	LARGE_INTEGER elapsedMicros;
	elapsedMicros.QuadPart = endTime.QuadPart - startTime.QuadPart;
	elapsedMicros.QuadPart *= 1000000;
	elapsedMicros.QuadPart /= frequency.QuadPart;

	return elapsedMicros.QuadPart;
}