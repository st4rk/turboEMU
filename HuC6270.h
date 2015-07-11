#ifndef HUC_6270_H
#define HUC_6270_H

#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include "MMU.h"

#define SCREEN_WIDTH  512
#define SCREEN_HEIGHT 512

#define SET_FLAG(f, n) (f | n)
#define CLEAR_FLAG(f, n) (f & ~n)

#define BSY 0x64
#define INT_VRAM_SATB 0x1
#define INT_VRAM_VRMA 0x2
#define DMA_TYPE_1    0x4
#define DMA_TYPE_2    0x8

/*	Direct Memory Access
	DCR  = DMA Control Register
	SOUR = Source Address Register
	DESR = Destination Address Register
	LENR = Block Lenght Register
*/

typedef struct dma {
	unsigned short DCR;
	unsigned short SOUR;
	unsigned short DESR;
	unsigned short LENR;
} dma;


class MMU;

class HuC6270 {

public:
	HuC6270(MMU *ptr);
	~HuC6270();

	bool initVideo();

	void writeVDC(unsigned short addr, unsigned char data);
	void dmaChannel_VRAM();
	void dmaChannel_STAB();

	unsigned char readVDC(unsigned short addr);

private:

	SDL_Window* mainWindow;
	SDL_Renderer *mainRenderer;
	MMU *memory;
	dma DMA;
	// there are 20 registers
	// however some are unknown ie:
	// register 3, 4
		
	// Memory Access Write Register
	// Memory Access Read  Register
	unsigned short MAWR;
	unsigned short MARR;
	// VRAM Read  Register
	// VRAM Write Register
	unsigned short VRR;
	unsigned short VWR;
	// Control Register
	unsigned short CR;
	// Raster Counter Register
	unsigned short RCR;
	// Background Scroll-X Register
	// Background Scroll-Y Register
	unsigned short BXR;
	unsigned short BYR;
	// Memory-Access Width Register
	unsigned short MWR;
	// Horizontal Sync Register
	// Horizontal Display Register
	unsigned short HSR;
	unsigned short HDR;
	// Vertical Synchronous Register
	// Vertical Display Register
	// Vertical Display End position register
	unsigned short VPR;
	unsigned short VDW;
	unsigned short VCR;
	// Sprite Attribute Table
	unsigned short SATB;

	// Registers shared with MMU
	unsigned char vdcStatus; // and ADDR 
    unsigned char vdcDataL;  
    unsigned char vdcDataM;  

};


#endif