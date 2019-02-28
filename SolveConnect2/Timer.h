#pragma once
#include <string>
#include <map>


class Timer
{
public:
	Timer() = delete;
	
public:
	static void addTime(std::string id, int time);
	static void addTimeTotal(int time);
	static std::string getStats();

private:
	static std::map<std::string, int> m_stats;
	static long long m_timeTotal;
};
