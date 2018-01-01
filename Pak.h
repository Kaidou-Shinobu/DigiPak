#pragma once

#define _SCL_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <memory>
#include <sstream>



//constexpr std::uint32_t MSGTYPE =	0x01000000;
constexpr std::uint32_t NFTRTYPE =	0x01000000;
constexpr std::uint32_t NCGRTYPE =	0x5247434E;
constexpr std::uint32_t NCLRTYPE =	0x524C434E;

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
	std::string hexToStr(std::uint32_t& data);

	
public:
	Pak() {};
	virtual ~Pak() = default;
	virtual void populate(std::ifstream& inputFILE) = 0;

	const PakHeader getHeader() const { return header; };

	void copyHeader(const PakHeader& hdr) {
		header = hdr;
	}

	virtual void import(std::string& jsonFilename) {};
};