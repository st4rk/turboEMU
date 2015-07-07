

#include "HuC6280.h"


int main(int argc, char **argv) {
	HuC6280 mainCore;

	std::cout << "turbo EMU - Written by St4rk" << std::endl;
	std::cout << "Loading PCE..." << std::endl;
	mainCore.setupROM();
	mainCore.resetCPU();

	while (true) {
		mainCore.executeCPU();
	}

	return 0;
}