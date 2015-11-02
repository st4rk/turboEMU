

#include "HuC6280.h"
#include "HuC6270.h"
#include "MMU.h"
#include "render.h"

int main(int argc, char **argv) {
	MMU mMMU;
	render mRender;
	HuC6280 CPU(&mMMU);
	HuC6270 VDC(&mMMU);

	mMMU.setupVDC(&VDC);
	mMMU.setupCPU(&CPU);
	mMMU.setupRender(&mRender);
	
	std::cout << "turbo EMU - Written by St4rk" << std::endl;
	std::cout << "Loading PCE..." << std::endl;
	mRender.initVideo();
	CPU.setupROM();
	CPU.resetCPU();


	while (1) {
		mRender.handleKeyboard();
		CPU.executeCPU();
		mRender.renderScene();
	}

	return 0;
}