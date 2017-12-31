#include "NCGRPak.h"

NCGRPak::NCGRPak(std::string& inputPakFilename) : 
Pak(inputPakFilename)
{
	if (inputPAKFILE.is_open()) {
		populate();
		inputPAKFILE.close();
	}
}

void NCGRPak::populate() {
	graphics.reserve(header.numOfFiles); //We know a priori the number of files in this PKG.

	//Populate the pointer lists
	for (unsigned int i = 0; i < header.numOfFiles; i++) {
		NCGRFile file;
		inputPAKFILE.read(reinterpret_cast<char*>(&file.fileOffset),			sizeof(file.fileOffset));
		inputPAKFILE.read(reinterpret_cast<char*>(&file.unknown),				sizeof(file.unknown));
		inputPAKFILE.read(reinterpret_cast<char*>(&file.sizeNoFileEndPadding),	sizeof(file.sizeNoFileEndPadding));
		inputPAKFILE.read(reinterpret_cast<char*>(&file.padding),				sizeof(file.padding));

		graphics.emplace_back(std::move(file));
		
	}
	//Calculate the total size taken by each file including padding
	for (unsigned int i = 0; i < header.numOfFiles - 1; i++) {
		graphics[i].sizeWithFileEndPadding = graphics[i + 1].fileOffset - graphics[i].fileOffset;
	}
	//Not sure if this is true yet
	graphics[header.numOfFiles - 1].sizeWithFileEndPadding = graphics[header.numOfFiles - 1].sizeNoFileEndPadding;

	for (auto& graphic : graphics) {
		graphic.data = std::make_unique<char[]>(graphic.sizeWithFileEndPadding);
		inputPAKFILE.read(graphic.data.get(), graphic.sizeWithFileEndPadding);		
	}

}

void NCGRPak::exportFiles() {
	
	for (unsigned int i = 0; i < graphics.size(); i++) {
		std::string outputNCGRFilename = "data/bg/BG_" + std::to_string(i) + ".ncgr";
		outputPAKFILE.open(outputNCGRFilename, std::ios::binary);
		outputPAKFILE.write(graphics[i].data.get(), graphics[i].sizeWithFileEndPadding);
		outputPAKFILE.close();
	}
	
}