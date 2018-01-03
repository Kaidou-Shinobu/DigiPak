#pragma once

#define _SCL_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <memory>
#include <sstream>
#include <tuple>
#include <experimental\filesystem>



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

struct NDSFile {
	std::uint32_t fileOffset;
	std::uint32_t unCompressedSize;
	std::uint32_t size;
	std::uint32_t bCompressed;
	std::unique_ptr<char[]> data;
};

class Pak {
private:
protected:
	PakHeader header;

	std::string extension;

	std::vector<NDSFile> files;

	std::string hexToStr(std::uint32_t& data);
	virtual std::unique_ptr<char[]> decompressPrototype(char* compressedBuffer, int uncompressedSize);
	
public:
	Pak();
	virtual ~Pak() {};
	virtual void populate(std::ifstream& inputFILE) {};

	const PakHeader getHeader() const { return header; };

	void copyHeader(const PakHeader& hdr) {
		header = hdr;
	}

	void setExtension(std::string ext) { extension = ext; }

	virtual int getNumFiles() { return files.size(); }

	virtual void import(std::string filename) {};
};