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

	// Debug Purpose
	printf("MPR Num: 0x%X\n", MPR);

	printf("Addr: 0x%X\n", addr);

	// HuCardROM First Page, IRQ2, IRQ1, TIMER, NMI and RESET Vector
	/*if (MPR == 0x00) {
		return HuCardROM[(addr & 0x1FFF)];
	}
	*/

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
		
	}

	std::cout << "Invalid MPR  ? Value: " << MPR << std::endl;
	return 0xFF;
}

unsigned char MMU::readMPRi(unsigned char n) { return (mpr[n]); }
unsigned char MMU::getTimerStart() { return timerStart; }
unsigned char MMU::readStack(unsigned short addr) { return wram[addr]; }

void MMU::writeMemory(unsigned short addr, unsigned char data) {
	unsigned char MPR = mpr[((addr >> 13) & 0xFF)];

	// HuCard ROM
	if ((MPR >= 0x00) && (MPR <= 0xF7)) {
		std::cout << "Probably error" << std::endl;
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
		writeIO((addr & 0x1FFF), data);
	}

}


void MMU::writeIO(unsigned short addr, unsigned char data) {

	// HuC6270 Ports /CE7
	if ((addr >= 0x0) && (addr <= 0x3FF)) {

	}

	// HuC6260 Ports /CEK
	if ((addr >= 0x400) && (addr <= 0x7FF)) {

	}

	// PSG Ports /CEP
	if ((addr >= 0x800) && (addr <= 0xBFF)) {

	}

	// Timer /CET
	if ((addr >= 0xC00) && (addr <= 0xFFF)) {
		// is only used the bits 0 ~ 6
		if (addr == 0x0C00)
			timerStart = (data & 0x7F) + 1;

		// only bit 0 is used ! 
		if (addr == 0x0C01)
			timerEnable = (data & 0x1);
	}

	// I/O Ports /CEIO
	if ((addr >= 0x1000) && (addr <= 0x13FF)) {

	}

	// Interrupt Request / Disable Registers /CECG
	if ((addr >= 0x1400) && (addr <= 0x17FF)) {

	}

	// Beyond it is reserverd for expansion.
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
	memset(&wram, 0x0, 0x7FFF);
}

void MMU::writeStack(unsigned short addr, unsigned char data) {
	wram[addr] = data;
}


bool MMU::isTimerEnable() { return timerEnable; }

bool MMU::startMemory() {
	if (pceLoader.PCE_LoadFile("test.pce")) {
		std::memcpy(HuCardROM, pceLoader.buffer, pceLoader.size);
		return true;
	} else 
		return false;
}