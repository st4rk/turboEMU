#include "MMU.h"



MMU::MMU() {
	log = fopen("log.txt", "w");
}

MMU::~MMU() {
	fclose(log);
	log = NULL;
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

// TODO: All Mirror

unsigned char MMU::readMemory(unsigned short addr) {
	unsigned char MPR = mpr[((addr >> 13) & 0xFF)];
	char debug[20];

	// Debug Purpose
	sprintf(debug, "MPR Num: 0x%X  Addr: 0x%X\n", MPR, addr);
	writeLog(debug);

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

	// Interrupt, timers, I/O ports
	// VDC/VCE(HuC6270 and HuC6260)
	if (MPR == 0xFF) {
		// Region map 0x0000 ~ 0x1FFF
		addr = (addr & 0x1FFF);
		// VDC (Video Display Controller)
		// registers mirrored every 4 bytes
		if ((addr >= 0x0000) && (addr <= 0x03FF)) {

		}

		// VCE (Video Color Encoder)
		// registers mirrored every 8 bytes
		if ((addr >= 0x4000) && (addr <= 0x07FF)) {

		}

		// PSG (Programmable Sound Generator)
		if ((addr >= 0x0800) && (addr <= 0x0BFF)) {

		}

		// Timer 
		// registers mirrored every 2 bytes
		if ((addr >= 0x0C00) && (addr <= 0x0FFF)) {
			// 0x0C00 Timer Counter/Value
			if (addr == 0x0C00)
				return timerStart;	

			// 0x0C01 Time Enable
			if (addr == 0x0C01)
				return timerEnable;
		}

		// I/O port 
		// mirrored every 2 bytes
		if ((addr >= 0x1000) && (addr <= 0x13FF)) {

		}


		// Interrupt Control Register
		// registers mirrored every 4 bytes 
		if ((addr >= 0x1400) && (addr <= 0x17FF)) {
			// 0x1402 - IRQ MASK
			// bit 0 IRQ2D = enable/disable
			// bit 1 IRQ1D = enable/disable
			// bit 2 TIQD  = enable/disable
			// bit 3 ~ 7 unused
			if (addr == 0x1402) 
				std::cout << "IRQ Mask" << std::endl;

			// 0x1403 - Interrupt status
			if (addr == 0x1403)
				std::cout << "Interrupt Status" << std::endl;
		}

		// 0x1800 ~ 0x1FFF always return FF
		// however there is documentation talking about
		// the region 0x1800 ~ 0x1BFF that is the CD-ROM Section
		return 0xFF;
		
	}

	std::cout << "Invalid MPR  ? Value: " << MPR << std::endl;
	return 0xFF;
}

unsigned char MMU::readMPRi(unsigned char n) { return (mpr[n]); }
unsigned char MMU::getTimerStart() { return timerStart; }
unsigned char MMU::readStack(unsigned short addr) { return wram[addr]; }

void MMU::writeMemory(unsigned short addr, unsigned char data) {
	unsigned char MPR = mpr[((addr >> 13) & 0xFF)];
	char debug[20];

	// HuCard ROM
	if ((MPR >= 0x00) && (MPR <= 0xF7)) {
		// Only for debug purpose

		writeLog("Trying write on HuCardROM !\n");
		sprintf(debug, "Addr: 0x%X  - Data: 0x%X\n", addr, data);
		writeLog(debug);

	}

	// WRAM
	if ((MPR >= 0xF8) && (MPR <= 0xFB)) {
		wram[(addr & 0x7FFF)] = data;
	}

	// Unused
	if ((MPR >= 0xFC) && (MPR <= 0xFE)) {
		printf("Write on unused, data: 0x%X\n", data);
	}

	// Hardware Page
	// Interrupt, timers, I/O etc...
	if (MPR == 0xFF) {
		writeIO((addr & 0x1FFF), data);
	}

}


// There are instroctuions (STO, ST1 and ST2) which
// written direct to the registers of HuC6270
// besides this we have the writeMemory 
void MMU::writeIO(unsigned short addr, unsigned char data) {
	// VDC (Video Display Controller)
	// registers mirrored every 4 bytes
	if ((addr >= 0x0000) && (addr <= 0x03FF)) {

	}

	// VCE (Video Color Encoder)
	// registers mirrored every 8 bytes
	if ((addr >= 0x4000) && (addr <= 0x07FF)) {

	}

	// PSG (Programmable Sound Generator)
	if ((addr >= 0x0800) && (addr <= 0x0BFF)) {

	}

	// Timer 
	// registers mirrored every 2 bytes
	if ((addr >= 0x0C00) && (addr <= 0x0FFF)) {
		// 0x0C00 Timer Counter/Value
		if (addr == 0x0C00)
			timerStart = data;

		// 0x0C01 Time Enable
		if (addr == 0x0C01)
			timerEnable = (addr & 0x1);
	}

	// I/O port 
	// mirrored every 2 bytes
	if ((addr >= 0x1000) && (addr <= 0x13FF)) {
		// 0x1000 Joypad
		if (addr == 0x1000) 
			std::cout << "joypad" << std::endl;

	}


	// Interrupt Control Register
	// registers mirrored every 4 bytes
	if ((addr >= 0x1400) && (addr <= 0x17FF)) {
		// 0x1402 - IRQ MASK
		// bit 0 IRQ2 = enable/disable
		// bit 1 IRQ1 = enable/disable
		// bit 2 timer  = enable/disable
		// bit 3 ~ 7 unused
		if (addr == 0x1402) 
			std::cout << "IRQ Mask" << std::endl;

		// 0x1403 - timer interrupt ?

		if (addr == 0x1403)
			std::cout << "Timer" << std::endl;

	}
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

void MMU::writeLog(std::string text) {
	fprintf(log, text.c_str());
}


bool MMU::isTimerEnable() { return timerEnable; }

bool MMU::startMemory() {
	if (pceLoader.PCE_LoadFile("test.pce")) {
		std::memcpy(HuCardROM, pceLoader.buffer, pceLoader.size);
		return true;
	} else 
		return false;
}