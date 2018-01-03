#include "GenericPak.h"

void GenericPak::populate(std::ifstream& inputFILE) {
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
void GenericPak::exportFile(int f) {

}

void GenericPak::exportAll(std::string filename) {
	std::ofstream outputNCLRFILE;
	std::experimental::filesystem::path thisPath(filename);
	std::experimental::filesystem::path rootName = thisPath.stem();
	std::experimental::filesystem::create_directory(rootName);
	for (unsigned int i = 0; i < files.size(); i++) {
		outputNCLRFILE.open(rootName.string() + "/" + rootName.string() + std::to_string(i) + "." + extension, std::ios::binary);
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

void GenericPak::import(std::string dir) {
	std::string newFilename = dir + ".PAK";
	
	std::experimental::filesystem::path tarPath(dir);
	const 	std::experimental::filesystem::directory_iterator end{};

	//std::vector<std::tuple<std::unique_ptr<char[]>, std::uint32_t>> dataBuffers;

	std::ifstream fileFILE;
	for (std::experimental::filesystem::directory_iterator dirIter(tarPath); dirIter != end; dirIter++) {
		NDSFile file;
		fileFILE.open(*dirIter);
		file.unCompressedSize = std::experimental::filesystem::file_size(*dirIter);
		file.size = file.unCompressedSize;
		file.bCompressed = 0x80000000;

		file.data = std::make_unique<char[]>(file.size);
		fileFILE.read(file.data.get(), static_cast<std::uint32_t>(file.size));
		fileFILE.close();
		//dataBuffers.emplace_back(std::forward_as_tuple<std::unique_ptr<char[], std::uint32_t>>(buffer, fileSize));
		files.emplace_back(std::move(file));
	}

	//Create Pak Header
	header.numOfFiles = files.size();
	header.version = 0x31302e32;
	header.junk = 0;
	header.junk2 = 0;
	std::ofstream outputPAKFILE;
	outputPAKFILE.open(newFilename, std::ios::binary);
	outputPAKFILE.write(reinterpret_cast<char*>(&header.numOfFiles), sizeof(std::uint32_t));
	outputPAKFILE.write(reinterpret_cast<char*>(&header.version), sizeof(std::uint32_t));
	outputPAKFILE.write(reinterpret_cast<char*>(&header.junk), sizeof(std::uint32_t));
	outputPAKFILE.write(reinterpret_cast<char*>(&header.junk2), sizeof(std::uint32_t));

	//Create Pointers
	std::uint32_t offset = header.numOfFiles * 16 + 16;
	for (auto& file : files) {
		file.fileOffset = offset;
		outputPAKFILE.write(reinterpret_cast<char*>(&file.fileOffset), sizeof(std::uint32_t));
		outputPAKFILE.write(reinterpret_cast<char*>(&file.size), sizeof(std::uint32_t));
		outputPAKFILE.write(reinterpret_cast<char*>(&file.unCompressedSize), sizeof(std::uint32_t));
		outputPAKFILE.write(reinterpret_cast<char*>(&file.bCompressed), sizeof(std::uint32_t));
		offset += file.unCompressedSize;
	}
	//Dump data
	for (auto& file : files) {
		outputPAKFILE.write(file.data.get(), file.unCompressedSize);
	}

	outputPAKFILE.close();
}