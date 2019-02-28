#include "Timer.h"


std::map<std::string, int> Timer::m_stats;
long long Timer::m_timeTotal;

void Timer::addTime(std::string id, int time)
{
	m_stats[id] += time;
}

void Timer::addTimeTotal(int time)
{
	m_timeTotal += time;
}

std::string Timer::getStats()
{
	int total = 0;
	for (auto it : m_stats) {
		total += it.second;
	}

	std::string ret;
	for (auto it : m_stats) {
		ret += it.first + " = " + std::to_string(it.second / (double)m_timeTotal * 100) + "%\n";
	}
	ret += "total = " + std::to_string(total / (double)m_timeTotal * 100) + "%\n";

	std::string totalTimeStr;
	if (m_timeTotal >= 3600000000)
		totalTimeStr = std::to_string(m_timeTotal / (double)(3600000000)) + " hours";
	else if (m_timeTotal >= 60000000)
		totalTimeStr = std::to_string(m_timeTotal / (double)(60000000)) + " minutes";
	else
		totalTimeStr = std::to_string(m_timeTotal / (double)(1000000)) + " seconds";
	ret += "time = " + totalTimeStr + "\n";

	return ret;
}
