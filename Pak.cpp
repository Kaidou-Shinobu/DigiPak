#include "Pak.h"

Pak::Pak(std::string& inputPakFilename) {
	inputPAKFILE.open(inputPakFilename, std::ios::binary);
	if (inputPAKFILE.is_open()) {
		std::cout << "File successfully opened.\n";

		//Prepopulate the Header. All Pak files will need to do this.
		inputPAKFILE.read(reinterpret_cast<char*>(&header.numOfFiles), sizeof(header.numOfFiles));
		inputPAKFILE.read(reinterpret_cast<char*>(&header.version), sizeof(header.version));
		inputPAKFILE.read(reinterpret_cast<char*>(&header.junk), sizeof(header.junk));
		inputPAKFILE.read(reinterpret_cast<char*>(&header.junk2), sizeof(header.junk2));

		std::cout <<
			header.numOfFiles << " " << hexToStr(header.version) << " | ";

		std::cout << std::hex <<
			header.numOfFiles << " " << std::hex << header.version << "\n";
	}
	else {
		std::cout << "File could not be opened.\n";
	}
}


std::string Pak::hexToStr(std::uint32_t& data) {

	std::string newString;
	char* ptr = reinterpret_cast<char*>(&data);
	newString.push_back(*ptr);
	newString.push_back(*(ptr + 1));
	newString.push_back(*(ptr + 2));
	newString.push_back(*(ptr + 3));
	return newString;
}