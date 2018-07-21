#include <iostream>

#include "MsgPak.h"
#include "GenericPak.h"
#include "PakManager.h"


int main(int argc, char** argv) {
	
	if (argc == 1) {
		std::cout << "Usage:\n";

		std::cout << "To Export to binary/JSON\n";
		std::cout << "./DigiPak [/path/to/file.PAK]\n\n";

		std::cout << "To Import to PAK\n";
		std::cout << "./DigiPak [/dir/to/files/]\n";

		std::cout << "You can also simply just click and drag the file or directory onto the executable.";
		return 0;
	}

	if (argc != 2) {
		std::cerr << "Please specify file.\n";
		return 1;
	}
	std::string filename = argv[1];
	

	std::cout << "VERSION v0.1.3a\n";
	std::cout << "Would you like to: (1) UNPAK or (2) PAK?\n";
	std::string userInput;
	std::cin >> userInput;
	if (userInput != "1" && userInput != "2") {
		std::cout << "Not an acceptable input. Program will now close.\n";
		return 1;
	}
	else {
		PakManager mngr;
		if (userInput == "1") { //Unpack me
			std::shared_ptr<Pak> pak = mngr.loadPak(filename);
			MsgPak* msgPak = dynamic_cast<MsgPak*>(pak.get());
			if (pak == nullptr) { //EXPORTING FROM PAK
				std::cout << "Error loading PAK file\n";
				return 1;
			}
			std::cout << "Is this a MSG Pak file?: (1) Yes or (2) No\n";
			std::cin >> userInput;
			if (userInput != "1" && userInput != "2") {
				std::cout << "Not an acceptable input. Program will now close.\n";
				return 1;
			}
			if (userInput == "1") {
				MsgPak* msgPak = dynamic_cast<MsgPak*>(pak.get());
				msgPak->exportAsJSON(filename);
			}
			else {
				std::shared_ptr<GenericPak> gPak = std::dynamic_pointer_cast<GenericPak>(pak);
				gPak->exportAll(filename);
				std::cout << gPak->getNumFiles() + " files successfully exported\n";
			}
		}
		else { //IMPORT TO PAK
			std::cout << "Are you importing into a MSG PAK file?: (1) Yes or (2) No\n";
			std::cin >> userInput;
			if (userInput != "1" && userInput != "2") {
				std::cout << "Not an acceptable input. Program will now close.\n";
				return 1;
			}
			if (userInput == "1") {
				std::unique_ptr<MsgPak> msgPak = std::make_unique<MsgPak>();
				msgPak->import(filename);
			}
			else {
				std::unique_ptr<GenericPak> gPak = std::make_unique<GenericPak>();
				gPak->import(filename);
			}
		}
	}
	return 0;
}