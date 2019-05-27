#pragma once
#include <string>


class Misc
{
public:
	Misc() = delete;

	static std::string padStr(std::string str, int amount = 16);
	static std::string padStrRight(std::string str, int amount = 16);
	static std::string formatIntWithCommas(int v);
};
