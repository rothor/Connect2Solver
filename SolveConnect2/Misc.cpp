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
	int numDigits = ceil(log10(v + 1));
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

#include <string> // for 'split' function
#include <sstream> // for 'split' function
#include <list> // for 'split' function
#include <iterator> // for 'split' function

template<typename Out>
void split_tem(const std::string& s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::list<std::string> Misc::split(const std::string& s, char delim) {
	std::list<std::string> elems;
	split_tem(s, delim, std::back_inserter(elems));
	return elems;
}