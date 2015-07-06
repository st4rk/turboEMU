#include "MMU.h"



MMU::MMU() {

}

MMU::~MMU() {

}


// The CPU can handle 2^16(PC is 16 bits), this is equal
// 64KB, however the internal memory has 2MB(2^21), to solve
// this problem, we use the first three bits of logical space
// to select the MPR/Memory Segment and with it, we do an OR
// with the logical space and it will give to us 21 bits

// Memory Segments
// Segment 0xFF        -> IO RAM
// Segment 0xFC ~ 0xFE -> unused
// Segment 0xF8 ~ 0xFB -> WRAM (segment F9,FA and FB mirror segment FB)
// Segment 0x00 ~ 0xF7 -> HuCard ROM

unsigned char MMU::readMemory(unsigned short addr) {
	unsigned char MPR = mpr[((addr >> 13) & 0xFF)];


	// HuCard ROM
	if ((MPR >= 0x00) && (MPR <= 0xF7)) {
		return HuCardROM[(addr & 0xFFFF)];
	}

	// WRAM

	if ((MPR >= 0xF8) && (MPR <= 0xFB)) {
		return wram[(addr & 0x7FFF)];
	}

	// Unused

	if ((MPR >= 0xFC) && (MPR <= 0xFE)) {
		return 0xFF;
	}

	// IO RAM

	if (MPR == 0xFF) {
		return io_ram[(addr & 0xFFFF)];
	}

	std::cout << "Invalid MPR  ? Value: " << MPR << std::endl;
	return 0xFF;
}

unsigned char MMU::readMPRi(unsigned char n) {
	return (mpr[n]);
}

void MMU::writeMemory(unsigned short addr, unsigned char data) {
	unsigned char MPR = mpr[((addr >> 13) & 0xFF)];

	// HuCard ROM
	if ((MPR >= 0x00) && (MPR <= 0xF7)) {
		HuCardROM[(addr & 0xFFFF)] = data;
	}

	// WRAM

	if ((MPR >= 0xF8) && (MPR <= 0xFB)) {
		wram[(addr & 0x7FFF)] = data;
	}

	// Unused
	if ((MPR >= 0xFC) && (MPR <= 0xFE)) {
		printf("Write on unused, data: 0x%X\n", data);
	}

	// IO RAM

	if (MPR == 0xFF) {
		io_ram[(addr & 0xFFFF)] = data;
	}

}


void MMU::writeIO(unsigned short addr, unsigned char data) {
	io_ram[addr] = data;
}


void MMU::setMPRi(unsigned char n, unsigned char data) {
	mpr[n] = data;
}

// It will clear all memory segments
// and will clear the memory mapping registers
// normally the only MPR7 is set to 0 and the 
// other has random numbers, in this case
// we will use 0 to all.
void MMU::clearMPR() {
	memset(&mpr, 0x0, 0x7);
	memset(&io_ram, 0x0, 0x1FFF);
	memset(&wram, 0x0, 0x7FFF);
	memset(&HuCardROM, 0x0, 0xFFFF);

}