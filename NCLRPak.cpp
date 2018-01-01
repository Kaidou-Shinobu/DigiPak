#include "NCLRPak.h"

void NCLRPak::populate(std::ifstream& inputFILE) {
	files.resize(header.numOfFiles); //We know a priori the number of files in this PKG.

	//Populate Pointer List
	for (auto& file : files) {
		inputFILE.read(reinterpret_cast<char*>(&file.fileOffset), sizeof(file.fileOffset));
		inputFILE.read(reinterpret_cast<char*>(&file.unCompressedSize), sizeof(file.unCompressedSize));
		inputFILE.read(reinterpret_cast<char*>(&file.size), sizeof(file.size));
		inputFILE.read(reinterpret_cast<char*>(&file.bCompressed), sizeof(file.bCompressed));
	}

	for (auto& file : files) {

		inputFILE.seekg(file.fileOffset);
		file.data = std::make_unique<char[]>(file.size);
		inputFILE.read(file.data.get(), file.size);
	}
}
void NCLRPak::exportFile(int f) {

}
void NCLRPak::exportAll() {
	std::ofstream outputNCLRFILE;
	for (unsigned int i = 0; i < files.size(); i++) {
		outputNCLRFILE.open("data/nclr/bg_" + std::to_string(i) + ".nclr", std::ios::binary);
		if (files[i].bCompressed == 0x00000000) {
			std::unique_ptr<char[]> uncompressedBuffer = decompressPrototype(files[i].data.get(), files[i].unCompressedSize);
			outputNCLRFILE.write(uncompressedBuffer.get(), files[i].unCompressedSize);
		}
		else {
			outputNCLRFILE.write(files[i].data.get(), files[i].unCompressedSize);
		}
		
		outputNCLRFILE.close();
	}
}