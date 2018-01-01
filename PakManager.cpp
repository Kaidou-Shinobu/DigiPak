#include "PakManager.h"

#include "NCGRPak.h"
#include "NFTRPak.h"
#include "MsgPak.h"


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
		std::cout << "File could not be opened.\n";
	}

	

	
	std::shared_ptr<Pak> pak;

	/*std::uint32_t pakMagicID;
	inputPAKFILE.read(reinterpret_cast<char*>(&pakMagicID), sizeof(pakMagicID));
	inputPAKFILE.seekg(-static_cast<std::int32_t>(sizeof(pakMagicID)), std::ios::cur);
	switch (pakMagicID) {
	case NCGRTYPE: pak = std::make_shared<NCGRPak>(); break;
	case NFTRTYPE: pak = std::make_shared<NFTRPak>(); break;
	default: pak = std::make_shared<MsgPak>(); break;
	}*/
	if (pakFilename.substr(pakFilename.size() - 12, 6) == "MESPAK") {
		pak = std::make_shared<MsgPak>();
	}
	else if (pakFilename.substr(pakFilename.size() - 8, 4) == "NCGR") {
		pak = std::make_shared<NCGRPak>();
	}
	else if (pakFilename.substr(pakFilename.size() - 8, 4) == "NFTR") {
		pak = std::make_shared<NFTRPak>();
	}
	else {
		std::cout << "Could not find correct PAK format!" << std::endl;
		return nullptr;
	}

	pak->copyHeader(header);
	pak->populate(inputPAKFILE);
	
	return pak;
}