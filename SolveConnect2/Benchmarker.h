#pragma once
#include <string>
#include <map>
#include <list>
#include "HighResolutionTimer.h"


class Benchmarker
{
public:
	Benchmarker();
	void clearAllTimes();
	void hardClearEverything();
	void clearTime(std::string id);
	void resetTimer(std::string id);
	void addTime(std::string id);
	std::string getStr(std::string name);
	std::string getPieChartStr(std::list<std::string> pieceName);
	std::string getTotalStr(std::string name);
	std::string getPieChartTotalStr(std::list<std::string> pieceName);

protected:
	int getLength();

	std::map<std::string, long long> m_stats;
	std::map<std::string, long long> m_statTotals;
	std::map<std::string, HighResolutionTimer> m_timers;
};
