#include <iostream>

#include "MsgPak.h"
#include "NCGRPak.h"


int main() {

	std::string inputPakFilename = "data/MESPAK01.PAK";
	MsgPak mainPak(inputPakFilename);
	std::string jsonFilename = "data/jsonExample.json";
	mainPak.exportAsJSON(jsonFilename);

	/*std::string inputPakFilename = "data/BG_NCGR.PAK";
	NCGRPak mainPak(inputPakFilename);
	mainPak.exportFiles();*/

	//std::cin.get();

	return 0;
}