#include <iostream>

#include "MsgPak.h"
#include "NCGRPak.h"
#include "PakManager.h"


int main() {
	PakManager mngr;
	//std::shared_ptr<Pak> pak = mngr.loadPak("data/MESPAK01.PAK");
	std::shared_ptr<Pak> pak = mngr.loadPak("data/BG_NCGR.PAK");
	/*std::string inputPakFilename = "data/BG_NCGR.PAK";
	MsgPak mainPak(inputPakFilename);
	std::string jsonFilename = "data/jsonExample.json";
	mainPak.exportAsJSON(jsonFilename);*/

	/*for (int i = 0; i <= 5; i++) {
		std::string inputPakFilename = "data/MESPAK0" + std::to_string(i) + ".PAK";
		MsgPak mainPak(inputPakFilename);
		std::string jsonFilename = "data/MESPAK0" + std::to_string(i) + ".json";
		mainPak.exportAsJSON(jsonFilename);
	}
	*/
	/*std::string inputPakFilename = "data/BG_NCGR.PAK";
	NCGRPak mainPak(inputPakFilename);
	mainPak.exportFiles();*/

	//std::cin.get();

	return 0;
}