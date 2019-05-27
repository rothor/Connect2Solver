#include "Misc.h"


std::string Misc::padStr(std::string str, int amount)
{
	while (str.size() < amount) {
		str = " " + str;
	}
	return str;
}

std::string Misc::padStrRight(std::string str, int amount)
{
	while (str.size() < amount) {
		str = str + " ";
	}
	return str;
}

std::string Misc::formatIntWithCommas(int v)
{
	if (v == 0)
		return std::string("0");
	std::string str = std::to_string(v);
	int numDigits = floor(log(v) / log(10)) + 1;
	int numCommas = numDigits / 3;
	int numLeftDigits = numDigits % 3;
	if (numLeftDigits == 0) {
		numCommas--;
		numLeftDigits = 3;
	}

	std::string ret;
	for (int i = 0; i < numLeftDigits; i++) { // Left-most digits
		ret += str[i];
	}
	for (int i = 0; i < numCommas; i++) { // Every group of 3 after that
		ret += ",";
		ret += str.substr(numLeftDigits + 3 * i, 3);
	}
	return ret;
}
