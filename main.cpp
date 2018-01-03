#include <iostream>

#include "MsgPak.h"
#include "GenericPak.h"
#include "PakManager.h"


int main(int argc, char** argv) {
	

	if (argc != 2) {
		std::cerr << "Please specify file.\n";
		return 1;
	}
	std::string filename = argv[1];
	
	
	//std::string filename = "BG_NGCR.PAK";
	//std::string filename = "data/MAP_NSCR.PAK";
	//std::string filename = "data/MESPAK01.json";
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
				std::shared_ptr<MsgPak> msgPak = std::dynamic_pointer_cast<MsgPak>(pak);
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
	std::cout << "Please press a button to continue." << std::endl;
	return 0;
}