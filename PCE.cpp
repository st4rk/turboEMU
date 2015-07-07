#include "PCE.h"

PCE::PCE() {
	size = 0;
}

PCE::~PCE() {
	if (size != 0)
		delete [] buffer;
	
	buffer = NULL;
	size = 0;
}

bool PCE::PCE_LoadFile(std::string fileName) {
	FILE* rom = nullptr;


	rom = fopen(fileName.c_str(), "rb");

	if (rom == NULL) {
		std::cout << "File not found: " << fileName << std::endl;
		return false;
	}


	fseek(rom, 0L, SEEK_END);
	size = ftell(rom);
	rewind(rom);


	if (size == 0) {
		std::cout << "Error, file size is 0" << std::endl;
		return false;
	}


	buffer = new unsigned char[size];

	fread(buffer, 1, size, rom);
	fclose(rom);

	return true;
}

