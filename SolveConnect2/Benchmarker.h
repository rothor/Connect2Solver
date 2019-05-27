#pragma once
#include <string>
#include <map>
#include <list>
#include "HighResolutionTimer.h"


class Benchmarker
{
public:
	Benchmarker() = delete;
	
public:
	static void clearAllTimes();
	static void clearTime(std::string id);
	static void resetTimer(std::string id);
	static void addTime(std::string id);
	static std::string getStr(std::string name);
	static std::string getPieChartStr(std::list<std::string> pieceName);
	static std::string getTotalStr(std::string name);
	static std::string getPieChartTotalStr(std::list<std::string> pieceName);

private:
	static int getLength();

	static std::map<std::string, long long> m_stats;
	static std::map<std::string, long long> m_statTotals;
	static std::map<std::string, HighResolutionTimer> m_timers;
};
