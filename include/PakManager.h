#pragma once

#include "Pak.h"

class PakManager {
private:
	std::ifstream inputPAKFILE;
public:
	PakManager() {};
	std::shared_ptr<Pak> loadPak(std::string& pakFilename);
};