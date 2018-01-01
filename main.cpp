#include <iostream>

#include "MsgPak.h"
#include "NCGRPak.h"
#include "NCLRPak.h"
#include "NSCRPak.h"
#include "PakManager.h"


int main() {
	PakManager mngr;
	std::shared_ptr<Pak> pak = mngr.loadPak("data/BG_NSCR.PAK");
	std::shared_ptr<NSCRPak> msgPak = std::dynamic_pointer_cast<NSCRPak>(pak);
	msgPak->exportAll();






	
	//std::string jsonFilename = "data/MESPAK01.json";
	
	//std::shared_ptr<Pak> pak = mngr.loadPak("data/MESPAK01.PAK");
	//std::shared_ptr<MsgPak> msgPak = std::dynamic_pointer_cast<MsgPak>(pak);
	//msgPak->exportAsJSON(jsonFilename);

	//std::shared_ptr<MsgPak> msgPak2 = std::make_shared<MsgPak>();
	//msgPak2->import(jsonFilename);


	return 0;
}