#pragma once
#include <string>
#include <list>


class Misc
{
public:
	Misc() = delete;

	static std::string padStr(std::string str, int amount);
	static std::string padStrRight(std::string str, int amount);
	static std::string formatIntWithCommas(int v);
	static std::string formatDoubleAsPercent(double d);
	static std::list<std::string> split(const std::string& s, char delim);
};
