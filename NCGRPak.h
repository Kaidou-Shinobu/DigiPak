#pragma once

#include "Pak.h"


struct NCGRFile {
	std::uint32_t fileOffset;
	std::uint32_t sizeUncompressed;
	std::uint32_t sizeNoFileEndPadding;
	std::uint32_t sizeWithFileEndPadding;
	std::uint32_t padding;
	std::unique_ptr<char[]> data;
};

class NCGRPak : public Pak {
private:
	virtual void populate(std::ifstream& inputFILE) override;
	std::vector<NCGRFile> graphics;

	//std::unique_ptr<char[]> decompressPrototype(NCGRFile& compressedFile);
	
public:
	NCGRPak() {};
	//std::string& inputPakFilename);
	void exportFiles();
	void importFiles(); //ToDo
};