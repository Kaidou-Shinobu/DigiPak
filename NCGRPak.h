#pragma once

#include "Pak.h"


struct NCGRFile {
	std::uint32_t fileOffset;
	std::uint32_t unknown;
	std::uint32_t sizeNoFileEndPadding;
	std::uint32_t sizeWithFileEndPadding;
	std::uint32_t padding;
	std::unique_ptr<char[]> data;
};

class NCGRPak : public Pak {
private:
	virtual void populate() override;
	std::vector<NCGRFile> graphics;
public:
	NCGRPak(std::string& inputPakFilename);
	void exportFiles();
};