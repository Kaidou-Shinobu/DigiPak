#pragma once

#include "Pak.h"


struct Message {
	std::uint32_t messageOffset;
	std::uint32_t size;
	std::unique_ptr<char[]> data;

};

struct MsgFile {
	std::uint32_t fileOffset;
	std::uint32_t numOfMessages;
	std::uint32_t size1;
	std::uint32_t size2; //Same as size1? Can probably just trim
	std::uint32_t padding; //Always 0x80000000?
	std::vector<Message> messages;
};


class MsgPak : public Pak {
private:
	
	std::vector<MsgFile> files;
	
	virtual void populate(std::ifstream& inputFILE) override;

public:
	MsgPak() {};
	void exportAsJSON(std::string& jsonFilename);
	virtual void import(std::string& jsonFilename) override;
};