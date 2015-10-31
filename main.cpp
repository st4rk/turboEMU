

#include "HuC6280.h"
#include "HuC6270.h"
#include "MMU.h"

int main(int argc, char **argv) {
	MMU mMMU;
	HuC6280 CPU(&mMMU);
	HuC6270 VDC(&mMMU);

	mMMU.setupVDC(&VDC);
	mMMU.setupCPU(&CPU);
	
	std::cout << "turbo EMU - Written by St4rk" << std::endl;
	std::cout << "Loading PCE..." << std::endl;
	CPU.setupROM();
	CPU.resetCPU();


	while (1) {
		CPU.executeCPU();
	}

	return 0;
}