#include "MsgPak.h"


void MsgPak::populate(std::ifstream& inputFILE) {

	if (!inputFILE.is_open()) {

	}

	files.resize(header.numOfFiles); //We know a priori the number of files in this PKG.

	//Populate Pointer List
	for (auto& file : files) {
		inputFILE.read(reinterpret_cast<char*>(&file.fileOffset), sizeof(file.fileOffset));
		inputFILE.read(reinterpret_cast<char*>(&file.size1), sizeof(file.size1));
		inputFILE.read(reinterpret_cast<char*>(&file.size2), sizeof(file.size2));
		inputFILE.read(reinterpret_cast<char*>(&file.padding), sizeof(file.padding));
	}
	int fileNo = 0;
	for (auto& file : files) {
		//Immediately should be 8 bytes of information regarding the number of Messages in the file
		std::uint32_t endAddr = file.fileOffset + file.size1;
		std::uint32_t temp;
		inputFILE.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		inputFILE.read(reinterpret_cast<char*>(&file.numOfMessages), sizeof(file.numOfMessages));

		file.messages.resize(file.numOfMessages);

		for (auto& message : file.messages) {
			inputFILE.read(reinterpret_cast<char*>(&message.messageOffset), sizeof(message.messageOffset));
		}

		for (unsigned int i = 0; i < file.numOfMessages - 1; i++) {

			file.messages[i].size = file.messages[i + 1].messageOffset - file.messages[i].messageOffset;
		}

		file.messages[file.numOfMessages - 1].size = file.size1 - file.messages[file.numOfMessages - 1].messageOffset;

		for (unsigned int i = 0; i < file.numOfMessages; i++) {
			file.messages[i].data = std::make_unique<char[]>(file.messages[i].size);
			inputFILE.read(file.messages[i].data.get(), file.messages[i].size);
		}
		fileNo++;
	}
	return;
}

//Sloppy Implementation for now
void MsgPak::import(std::string& jsonFilename) {
	std::ifstream inputPAKFILE;
	std::istringstream iss;
	inputPAKFILE.open(jsonFilename, std::ifstream::binary);
	std::string temp;
	std::getline(inputPAKFILE, temp); // {
	std::getline(inputPAKFILE, temp); // Line that has number of files
	iss = std::istringstream(temp);
	iss >> temp >> temp;

	header.numOfFiles = std::stoi(temp.substr(0, temp.size() - 1));
	if (header.numOfFiles == 0) {
		std::cout << "No files present\n";
	}
	
	files.reserve(header.numOfFiles);

	for (unsigned int f = 0; f < header.numOfFiles; f++) {
		MsgFile file;
		std::getline(inputPAKFILE, temp);
		std::getline(inputPAKFILE, temp);
		std::getline(inputPAKFILE, temp);
		iss = std::istringstream(temp);
		iss >> temp >> temp;
		file.numOfMessages = std::stoi(temp.substr(0, temp.size() - 1));
		for (unsigned int m = 0; m < file.numOfMessages; m++) {
			Message msg;
			int quoteCount = 0;
			char c;
			while (quoteCount < 3) {
				inputPAKFILE.get(c);
				if (c == '\"') {
					quoteCount++;
				}
			}

			std::vector<char> vBuffer;
			
			inputPAKFILE.get(c);
			do {
				vBuffer.push_back(c);
				inputPAKFILE.get(c);
			} while (c != '\"');
			//
			if (vBuffer.size() % 2 != 0) {
				vBuffer.push_back(static_cast<char>(0x00));
			}
			msg.size = vBuffer.size();
			msg.data = std::make_unique<char[]>(vBuffer.size());
			std::copy(vBuffer.begin(), vBuffer.end(), msg.data.get());

			file.messages.emplace_back(std::move(msg));

			if (m != file.numOfMessages - 1) {
				inputPAKFILE.seekg(3, std::ios::cur);
			}
			else {
				std::getline(inputPAKFILE, temp);
				std::getline(inputPAKFILE, temp);
			}
		}
		file.size1 = 0;
		file.size2 = 0;
		for (auto& msg : file.messages) {
			file.size1 += msg.size;
		}
		file.size2 = file.size1;
		file.padding = 0x80000000;

		files.emplace_back(std::move(file));
	}
	inputPAKFILE.close();

	std::ofstream outputPakFILE;
	std::string outputPakFilename = jsonFilename.substr(0, jsonFilename.size() - 4) + "edited.PAK";

	header.version = 0x31302e32;
	header.junk = 0;
	header.junk2 = 0;

	outputPakFILE.open(outputPakFilename, std::ofstream::binary);
	outputPakFILE.write(reinterpret_cast<char*>(&header.numOfFiles),	sizeof(std::uint32_t));
	outputPakFILE.write(reinterpret_cast<char*>(&header.version),		sizeof(std::uint32_t));
	outputPakFILE.write(reinterpret_cast<char*>(&header.junk),			sizeof(std::uint32_t));
	outputPakFILE.write(reinterpret_cast<char*>(&header.junk2),			sizeof(std::uint32_t));

	std::uint32_t offset = header.numOfFiles * 16 + 16;
	for (unsigned int f = 0; f < files.size(); f++) {
		MsgFile& file = files[f];
		file.fileOffset = offset;
		outputPakFILE.write(reinterpret_cast<char*>(&file.fileOffset),	sizeof(std::uint32_t));
		outputPakFILE.write(reinterpret_cast<char*>(&file.size1),		sizeof(std::uint32_t));
		outputPakFILE.write(reinterpret_cast<char*>(&file.size2),		sizeof(std::uint32_t));
		outputPakFILE.write(reinterpret_cast<char*>(&file.padding),		sizeof(std::uint32_t));
		offset += file.size1 + (file.numOfMessages * 4) + 4 + 4; 
	}
	
	for (auto& file : files) {
		std::uint32_t zeroPadding = 0x00000000;
		outputPakFILE.write(reinterpret_cast<char*>(&zeroPadding), sizeof(std::uint32_t));
		outputPakFILE.write(reinterpret_cast<char*>(&file.numOfMessages), sizeof(std::uint32_t));
		offset = 8 + (file.numOfMessages * 4);
		for (auto& message : file.messages) {
			message.messageOffset = offset;
			outputPakFILE.write(reinterpret_cast<char*>(&message.messageOffset), sizeof(std::uint32_t));
			offset += message.size;
		}
		for (unsigned int m = 0; m < file.numOfMessages; m++) {
			Message& message = file.messages[m];
			outputPakFILE.write(message.data.get(), message.size);
		}
	}



	outputPakFILE.close();


	return;
}

void MsgPak::exportAsJSON(std::string& jsonFilename) {
	std::ofstream outputPAKFILE;
	outputPAKFILE.open(jsonFilename, std::ios::binary);
	outputPAKFILE << "{";
	outputPAKFILE << "\n\t\"numFiles\": " << files.size() << ",";
	for (unsigned int f = 0; f < files.size(); f++) {
		outputPAKFILE << "\n\t\"File" + std::to_string(f) + "\": " << "\n\t{";
		outputPAKFILE << "\n\t\t\"numMessages\": " << files[f].messages.size() << ",";
		for (unsigned int m = 0; m < files[f].messages.size(); m++) {
			outputPAKFILE << "\n\t\t\"Msg" + std::to_string(m) + "\": ";
			outputPAKFILE << "\"";
			outputPAKFILE.write(files[f].messages[m].data.get(), files[f].messages[m].size);
			outputPAKFILE << "\"";
			if (m != files[f].messages.size() - 1) {
				outputPAKFILE << ",";
			}
		}

		outputPAKFILE << "\n\t}";
		if (f != files.size() - 1) {
			outputPAKFILE << ",";
		}
	}
	outputPAKFILE << "\n}";
	
	outputPAKFILE << "\"";
	outputPAKFILE.close();

}