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

		std::vector<std::uint32_t> messageSizes(file.numOfMessages);

		for (unsigned int i = 0; i < file.numOfMessages - 1; i++) {
			messageSizes[i] = file.messages[i + 1].messageOffset - file.messages[i].messageOffset;
		}
		messageSizes[file.numOfMessages - 1] = file.size1 - file.messages[file.numOfMessages - 1].messageOffset;

		for (unsigned int i = 0; i < file.numOfMessages; i++) {
			file.messages[i].data = std::make_unique<char[]>(messageSizes[i] + 1);
			inputFILE.read(file.messages[i].data.get(), messageSizes[i]);
			file.messages[i].data[messageSizes[i] - 1] = '\0';
		}
		fileNo++;
	}
	return;
}

void MsgPak::import(std::string& jsonFilename) {
	std::ifstream inputPAKFILE;
	inputPAKFILE.open(jsonFilename);
	if (!inputPAKFILE.is_open()) {
		return;
	}
	inputPAKFILE.close();


	return;
}

void MsgPak::exportAsJSON(std::string& jsonFilename) {
	std::ofstream outputPAKFILE;
	outputPAKFILE.open(jsonFilename);
	outputPAKFILE << "{";
	outputPAKFILE << "\n\t\"numFiles\": " << files.size() << ",";
	for (unsigned int f = 0; f < files.size(); f++) {
		outputPAKFILE << "\n\t\"File" + std::to_string(f) + "\": " << "\n\t{";
		outputPAKFILE << "\n\t\t\"numMessages\": " << files[f].messages.size() << ", ";
		for (unsigned int m = 0; m < files[f].messages.size(); m++) {
			outputPAKFILE << "\n\t\t\"Msg" + std::to_string(m) + "\": ";
			outputPAKFILE << "\"" << files[f].messages[m].data.get() << "\"";
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
	outputPAKFILE.close();

}