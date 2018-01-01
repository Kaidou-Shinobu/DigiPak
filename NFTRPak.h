#pragma once

#include "Pak.h"

class NFTRPak : public Pak {
private:
	void populate(std::ifstream& inputFILE) override;
public:
	NFTRPak() {};
};