#include "Pak.h"

std::string Pak::hexToStr(std::uint32_t& data) {

	std::string newString;
	char* ptr = reinterpret_cast<char*>(&data);
	newString.push_back(*ptr);
	newString.push_back(*(ptr + 1));
	newString.push_back(*(ptr + 2));
	newString.push_back(*(ptr + 3));
	return newString;
}