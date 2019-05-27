#include "Benchmarker.h"
#include "Misc.h"


std::map<std::string, long long> Benchmarker::m_stats;
std::map<std::string, long long> Benchmarker::m_statTotals;
std::map<std::string, HighResolutionTimer> Benchmarker::m_timers;

void Benchmarker::clearAllTimes()
{
	m_stats.clear();
	m_timers.clear();
	m_statTotals.clear();
}

void Benchmarker::clearTime(std::string id)
{
	m_stats[id] = 0;
}

void Benchmarker::resetTimer(std::string id)
{
	m_timers[id].reset();
}

void Benchmarker::addTime(std::string id)
{
	long long time = m_timers[id].readMicro();
	m_stats[id] += time;
	m_statTotals[id] += time;
}

std::string formatDouble(double d)
{
	std::string ret = std::to_string(d);
	int pos = ret.find_first_of('.');
	if (ret.size() - pos <= 3)
		return ret;
	return ret.substr(0, pos + 3);
}

std::string getTimeStr(long long microseconds)
{
	if (microseconds >= 3600000000) // microseconds in an hour
		return Misc::padStr(formatDouble(microseconds / (double)(3600000000)), 5) +
			" " + Misc::padStr("hours", 7);
	else if (microseconds >= 60000000) // microseconds in a minute
		return Misc::padStr(formatDouble(microseconds / (double)(60000000)), 5) +
			" " + Misc::padStr("minutes", 7);
	else
		return Misc::padStr(formatDouble(microseconds / (double)(1000000)), 5) +
			" " + Misc::padStr("seconds", 7);
}

std::string Benchmarker::getStr(std::string name)
{
	return std::string(Misc::padStrRight(name, getLength()) + " = " + getTimeStr(m_stats[name]));
}

std::string Benchmarker::getPieChartStr(std::list<std::string> pieceNameArr)
{
	long long sumTotal = 0;
	for (auto name : pieceNameArr) {
		sumTotal += m_stats[name];
	}

	std::string ret;
	int nameLength = getLength();
	ret += Misc::padStrRight("sum of parts", nameLength) + " = " +
		getTimeStr(sumTotal) + "\n";
	for (auto name : pieceNameArr) {
		ret += "\t" + Misc::padStrRight(name, nameLength) + " = " +
			getTimeStr(m_stats[name]) + ", " +
			Misc::padStr(formatDouble(m_stats[name] / (double)sumTotal * 100), 5) + "%\n";
	}

	return ret.substr(0, ret.size() - 1); // remove last \n
}

std::string Benchmarker::getTotalStr(std::string name)
{
	return std::string(Misc::padStrRight(name, getLength()) + " = " + getTimeStr(m_statTotals[name]));
}

std::string Benchmarker::getPieChartTotalStr(std::list<std::string> pieceNameArr)
{
	long long sumTotal = 0;
	for (auto name : pieceNameArr) {
		sumTotal += m_statTotals[name];
	}

	std::string ret;
	int nameLength = getLength();
	ret += Misc::padStrRight("sum of parts", nameLength) + " = " +
		getTimeStr(sumTotal) + "\n";
	for (auto name : pieceNameArr) {
		ret += "\t" + Misc::padStrRight(name, nameLength) + " = " +
			getTimeStr(m_statTotals[name]) + ", " +
			Misc::padStr(formatDouble(m_statTotals[name] / (double)sumTotal * 100), 5) + "%\n";
	}

	return ret.substr(0, ret.size() - 1); // remove last \n
}

int Benchmarker::getLength()
{
	int maxNameLength = 0;
	for (auto pair : m_stats) {
		if (pair.first.size() > maxNameLength)
			maxNameLength = pair.first.size();
	}
	return maxNameLength;
}