#include <iostream>

#include "MsgPak.h"
#include "GenericPak.h"
#include "PakManager.h"


int main() {
	PakManager mngr;
	//std::shared_ptr<Pak> pak = mngr.loadPak("data/BG_NCGR.PAK");
	//std::shared_ptr<Pak> pak = mngr.loadPak("data/BG_NCLR.PAK");
	//std::shared_ptr<Pak> pak = mngr.loadPak("data/BG_NSCR.PAK");
	//std::shared_ptr<Pak> pak = mngr.loadPak("data/BG_NCGR.PAK");
	std::shared_ptr<Pak> pak = mngr.loadPak("data/FONT_NFTR.PAK");

	if (pak == nullptr) {
		return 1;
	}

	//std::shared_ptr<NCGRPak> msgPak = std::dynamic_pointer_cast<NCGRPak>(pak);
	std::shared_ptr<GenericPak> gPak = std::dynamic_pointer_cast<GenericPak>(pak);
	gPak->exportAll();

	std::string jsonFilename = "data/MESPAK01.json";
	
	//std::shared_ptr<Pak> pak = mngr.loadPak("data/MESPAK01.PAK");
	//std::shared_ptr<MsgPak> msgPak = std::dynamic_pointer_cast<MsgPak>(pak);
	//msgPak->exportAsJSON(jsonFilename);
//
	std::shared_ptr<MsgPak> msgPak2 = std::make_shared<MsgPak>();
	msgPak2->import(jsonFilename);


	return 0;
}