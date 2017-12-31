#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <memory>

struct PakHeader { //32Byte Header
	std::uint32_t numOfFiles;
	std::uint32_t version;
	std::uint32_t junk;
	std::uint32_t junk2;
};

class Pak {
private:
protected:
	PakHeader header;

	std::ifstream inputPAKFILE;
	std::ofstream outputPAKFILE;

	std::string hexToStr(std::uint32_t& data);

	virtual void populate() = 0;
public:
	Pak(std::string& inputPakFilename);
};