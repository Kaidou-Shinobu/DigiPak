#include "PakManager.h"

#include "MsgPak.h"
#include "GenericPak.h"


std::shared_ptr<Pak> PakManager::loadPak(std::string pakFilename) {

	PakHeader header;

	inputPAKFILE.open(pakFilename, std::ios::binary);
	if (inputPAKFILE.is_open()) {
		std::cout << "File successfully opened.\n";

		//Prepopulate the Header. All Pak files will need to do this.
		inputPAKFILE.read(reinterpret_cast<char*>(&header.numOfFiles), sizeof(header.numOfFiles));
		inputPAKFILE.read(reinterpret_cast<char*>(&header.version), sizeof(header.version));
		inputPAKFILE.read(reinterpret_cast<char*>(&header.junk), sizeof(header.junk));
		inputPAKFILE.read(reinterpret_cast<char*>(&header.junk2), sizeof(header.junk2));
	}
	else {
		std::cout << "File could not be opened. Please press a button to exit.\n";
		std::cin.get();
		return nullptr;
	}
	
	std::shared_ptr<Pak> pak;

	if (pakFilename.size() >= 12 && pakFilename.substr(pakFilename.size() - 12, 6) == "MESPAK") {
		pak = std::make_shared<MsgPak>();
	}
	else {
		pak = std::make_unique<GenericPak>();
		if (pakFilename.size() >= 8) {
			//pak->setExtension(pakFilename.substr(pakFilename.size() - 8, 4));
			pak->setExtension("bin");
		}
	}

	pak->copyHeader(header);
	pak->populate(inputPAKFILE);
	
	return pak;
}