#pragma once

#include "Pak.h"

struct NSCRFile {
	std::uint32_t fileOffset;
	std::uint32_t unCompressedSize;
	std::uint32_t size;
	std::uint32_t bCompressed;
	std::unique_ptr<char[]> data;
};

class NSCRPak : public Pak {
private:
	std::vector<NSCRFile> files;
	virtual void populate(std::ifstream& inputFILE) override;

public:
	NSCRPak() {};
	void exportFile(int f);
	void exportAll();
};