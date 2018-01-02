#pragma once

#include "Pak.h"


struct Message {
	std::uint32_t messageOffset;
	std::uint32_t size;
	std::unique_ptr<char[]> data;

};

struct MsgFile {
	std::uint32_t fileOffset; //Poipnter first 4 bytes
	std::uint32_t size1; //Pointer second 4 bytes --Uncompressed
	std::uint32_t size2; //Pointer third 4 bytesSame as size1? Can probably just trim  --It's actually compressed size All Message files are uncompressed by default
	std::uint32_t padding; //Poipnter last 4 bytes -- Always 0x80000000?
	std::uint32_t numOfMessages;
	std::vector<Message> messages;
};


class MsgPak : public Pak {
private:
	
	std::vector<MsgFile> messageFiles;
	
	virtual void populate(std::ifstream& inputFILE) override;

public:
	MsgPak() {};
	void exportAsJSON(std::string& jsonFilename);
	virtual void import(std::string& jsonFilename) override;
};