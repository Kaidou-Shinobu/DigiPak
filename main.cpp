#include <iostream>
#include <string>
#include <tuple>
#include <map>

#include "MsgPak.h"
#include "GenericPak.h"
#include "PakManager.h"

void ProcessArgs(std::vector<std::pair<std::string, std::string>>& args);



int main(int argc, char** argv) {
	std::string VERSION = "VERSION v0.1.3a";
	std::cout << VERSION << "\n";

	std::map<std::string, std::string> options = {
		{ "-i", "filename for input." },
		{ "-u", "unpacking for non-MSG data." },
		{ "-um", "unpacking for MSG data." },
		{ "-p", "packing for non-MSG data." },
		{ "-pm", "packing for MSG data." },
	};

	if (argc == 1) {
		std::cout << "Usage:\n";

		std::cout << "Command-line arguments:\n\n";

		std::cout << "For making a PAK file:\n";

		std::cout << "For exporting from PAK file:\n\n";
		std::cout << "-i " << " " << options["-i"]	<< "\n";
		std::cout << "-u " << " " << options["-u"]  << "\n\n";
		std::cout << "-um" << " " << options["-um"]	<< "\n";

		std::cout << "For importing data/JSON file into PAK:\n";
		std::cout << "-i " << " " << options["-i"]   << "\n";
		std::cout << "-p " << " " << options["-p"]	 << "\n";
		std::cout << "-pm" << " " << options["-pm"]	 << "\n";

		return 0;
	}
	
	//Command line arguments
	if (argc % 2 == 0) {
		std::cout << "Incorrect specification of flags/arguments";
	}
	std::vector<std::pair<std::string, std::string>> arguments;
	
	for (int i = 1; i < argc-1; i+=2) {
		std::string flag = argv[i];
		std::string flag_arg = argv[i + 1];
		auto flag_arg_iter = options.find(flag);
		if (flag_arg_iter != options.end()) {
			arguments.emplace_back(flag, flag_arg);
		}
	}
	ProcessArgs(arguments);
	return 0;
}

void ProcessArgs(std::vector<std::pair<std::string, std::string>>& args) {
	std::string input;
	std::string output;
	//Preprocess
	bool actionIsClear = false;
	for (const auto& arg : args) {
		if (arg.first.substr(0, 2) == "-p" ||
			arg.first.substr(0, 2) == "-u") {
			if (actionIsClear) {
				std::cout << "Multiple packing/unpacking requests received. Exiting...\n";
				return;
			}
			actionIsClear = true;
		}
		if (arg.first == "-i" && input.empty()) {
			input = arg.second;
		}
		if ((arg.first.substr(0, 2) == "-u" || 
			arg.first.substr(0, 2) == "-p") && 
			output.empty()) {
			output = arg.second;
		}
	}
	for (const auto& arg : args) {
		switch (arg.first[1]) {
		case 'p': {
			if (arg.first.size() > 2 && arg.first[2] == 'm') {
				std::unique_ptr<MsgPak> msgPak = std::make_unique<MsgPak>();
				msgPak->import(input, output);
			}
			else {
				std::unique_ptr<GenericPak> gPak = std::make_unique<GenericPak>();
				gPak->import(input, output);
			}
			break;
		}
		case 'u': {
			PakManager mngr;
			std::shared_ptr<Pak> pak = mngr.loadPak(input);
			MsgPak* msgPak = dynamic_cast<MsgPak*>(pak.get());
			if (pak == nullptr) { //EXPORTING FROM PAK
				std::cout << "Error loading PAK file\n";
				return;
			}
			if (arg.first.size() > 2 && arg.first[2] == 'm') {
				MsgPak* msgPak = dynamic_cast<MsgPak*>(pak.get());
				msgPak->exportAsJSON(output);
			}
			else {
				std::shared_ptr<GenericPak> gPak = std::dynamic_pointer_cast<GenericPak>(pak);
				gPak->exportAll(output);
				std::cout << gPak->getNumFiles() + " files successfully exported\n";
			}
			break;
		}
		}
	}
}