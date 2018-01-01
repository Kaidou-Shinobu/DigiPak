#pragma once

#include "Pak.h"

struct NCLRFile {
	std::uint32_t fileOffset;
	std::uint32_t unCompressedSize;
	std::uint32_t size;
	std::uint32_t bCompressed;
	std::unique_ptr<char[]> data;
};

class NCLRPak : public Pak {
private:
	std::vector<NCLRFile> files;
	virtual void populate(std::ifstream& inputFILE) override;
	
public:
	NCLRPak() {};
	void exportFile(int f);
	void exportAll();
};