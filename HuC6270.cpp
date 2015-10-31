#include "HuC6270.h"



HuC6270::HuC6270(MMU *ptr) {
   memory = ptr;
   mainWindow   = NULL;
   mainRenderer = NULL;
   debugFont    = NULL;
}

HuC6270::~HuC6270() {
	SDL_DestroyRenderer (mainRenderer);
	SDL_DestroyWindow   (mainWindow);
	SDL_DestroyTexture  (debugText);

	mainRenderer = NULL;
	mainWindow   = NULL;
	debugText    = NULL;

	TTF_Quit();
	SDL_Quit();
}


bool HuC6270::initVideo() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error on SDL Init: " << SDL_GetError() << std::endl;
		return false;
	}

	mainWindow = SDL_CreateWindow("turboEMU rev 0.1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
									SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	if (mainWindow == NULL) {
		std::cout << "Error on SDL_CreateWindow, SDL_Error : " << SDL_GetError() << std::endl;
		return false;
	}


	mainRenderer = SDL_CreateRenderer (mainWindow, -1, SDL_RENDERER_ACCELERATED );

	if (mainRenderer == NULL) {
		std::cout << "error on SDL_CreateRenderer, SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}


	SDL_SetRenderDrawColor( mainRenderer, 0x00, 0xFF, 0x00, 0xFF);


	if ( TTF_Init() == -1) {
		std::cout << "Error on TTF Init ! SDL_Error: " << TTF_GetError() << std::endl;
		return false;
	}


	/* Load System TTF */

	debugFont = TTF_OpenFont("data/8bit16.ttf", 12);

	if (debugFont == NULL) {
		std::cout << "Error on TTF_OpenFont, SDL_Error:" << TTF_GetError() << std::endl;
		return false;
	}

	return true;
}

bool HuC6270::isSpriteEnable() { return (CR & 0x40); }

void HuC6270::writeVDC(unsigned short addr, unsigned char data) {
	printf("Write VDC: 0x%X\n", addr);
	switch (addr) {

		// VDC Address Select
		case 0x0000: // only bits 0-4, bits 5-7 are ignored 
			vdcStatus = (data & 0x1F);

			printf("Status: 0x%X\n", vdcStatus);
			switch (vdcStatus) {
				// MAWR - Memory Address Write Register
				case 0x0:
					MAWR = ((vdcDataL & 0x00FF) | (vdcDataM << 8));
				break;

				// MARR - Memory Address Read Register
				case 0x1:
					MARR = ((vdcDataL & 0x00FF) | (vdcDataM << 8));
				break;

				// VWR - VRAM Write Register
				case 0x2:
					std::cout << "WRITE ON VRAM" << std::endl;
					VWR = ((vdcDataL & 0x00FF) | (vdcDataM << 8));
					memory->writeVRAM(MAWR, VWR);
					
					// Auto-increment, flag IW of control register
					// 0x00 = Increment +1
					// 0x01 = Increment +32
					// 0x02 = Increment +64
					// 0x03 = Increment +128
					// The register should wrap to 0 after 0xFFFF

					if ((CR & FLAG_IW) == 0x00) {
						MAWR = ((MAWR + 1) & 0xFFFF);
					} else if ((CR & FLAG_IW) == 0x01) {
						MAWR = ((MAWR + 32) & 0xFFFF);
					} else if ((CR & FLAG_IW) == 0x02) {
						MAWR = ((MAWR + 64) & 0xFFFF);
					} else if ((CR & FLAG_IW) == 0x03) {
						MAWR = ((MAWR + 128) & 0xFFFF);
					}

				break;


				// These Registers are reserved
				case 0x3:

				break;

				case 0x4:

				break;
				// ---------------------------------

				// Control Register
				case 0x5:
					/*
					bit 0     = Sprite Collision Interrupt 
					bit 1     = Sprite Overflow Interrupt
					bit 2     = scanline interrupt flag
					bit 3     = vertical blanking interrupt flag
					bit 4-5   = reserved, always 0
					bit 6     = sprites enable
					bit 8-10  = reserved, always set to 0
					bit 11-12 = read/write address auto-increment
					source: http://www.magicengine.com/mkit/doc_hard_vdc.html
					*/
					CR = ((vdcDataL & 0x00FF) | (vdcDataM << 8));
				break;


				// Raster Counter Register
				case 0x6: // only bits 0~9 used
					RCR = (((vdcDataL & 0x00FF) | (vdcDataM << 8)) & 0x3FF);
				break;

				// Background X Scroll Register
				case 0x7:
					BXR = (((vdcDataL & 0x00FF) | (vdcDataM << 8)) & 0x3FF);
				break;

				// Background Y Scroll Register
				case 0x8:
					BYR = (((vdcDataL & 0x00FF) | (vdcDataM << 8)) & 0x1FF);
				break;

				// Memory Access Width Register
				case 0x9: 
					MWR = (((vdcDataL & 0x00FF) | (vdcDataM << 8)) & 0x70);
				break;

				// Horizontal Synchro Register
				case 0xA:

				break;

				// Horizontal Display Register
				case 0xB:

				break;

				// Vertical Synchro Register
				case 0xC:

				break;

				// Vertical Display Register
				case 0xD:

				break;

				// Vertical Display End Position Regiter
				case 0xE:

				break;

				/* DMA Control Register
					bit:
					0 = VRAM to SATB end of transfer interrupt enable flag (1 = on)
					1 = VRAM to VRAM end of transfer interrupt enable flag (1 = on)
					2 = Source Address Direction(1 = decrement, 0 = increment)
					3 = Destination Address Direction(1 = decrement, 0 = increment)
					4 = SATB DMA auto-transfer enable flag (1 = on)
				*/
				case 0xF:
					DMA.DCR  = ((vdcDataL & 0x00FF) | (vdcDataM << 8));
				break;

				// Source Address Register
				case 0x10:
					DMA.SOUR = ((vdcDataL & 0x00FF) | (vdcDataM << 8));
				break;

				// Destination Address Register
				case 0x11:
					DMA.DESR = ((vdcDataL & 0x00FF) | (vdcDataM << 8));
				break;

				// Block Lenght Register
				// Set value here start as soon the DMA  Transfer
				case 0x12:
					DMA.LENR =  ((vdcDataL & 0x00FF) | (vdcDataM << 8));
					dmaChannel_VRAM();
				break;

				// VRAM-SATB (Sprite Attribute Table Block)
				case 0x13:

				break;

				default:
					std::cout << "VDC Status Error" << std::endl;
				break;
			}
		break;

		// VDC Data LSB
		case 0x0002:
			vdcDataL = data;
		break;

		// VDC Data MSB
		case 0x0003:
			vdcDataM = data;
		break;

		default:
			std::cout << "VDC Write ERROR" << std::endl;
			printf("Addr: 0x%X | Data: 0x%X\n", addr, data);
		break;
	}
	


}

// There are two DMA Channel on PC Engine VDC, one is dedicated
// to transfer data between VRAM <-> VRAM and the another is dedicated
// to transfer data between VRAM <-> SATB

// Dedicated to VRAM <-> VRAM
void HuC6270::dmaChannel_VRAM() {

	unsigned char *VRAM = memory->getVRAM();

	unsigned short i = 0;

	while (i < DMA.LENR) {
		VRAM[DMA.SOUR + ((DMA.DCR & DMA_TYPE_2) == 0 ? 1 : -1)] = VRAM[DMA.DESR + ((DMA.DCR & DMA_TYPE_1) == 0 ? 1 : -1)];
		i++;
	}


}

// Dedicated to VRAM <-> STAB
void HuC6270::dmaChannel_STAB() {

}

unsigned char HuC6270::readVDC(unsigned short addr) {
	addr = (addr & 0x3);

	printf("Read VDC: 0x%X\n", addr);


	switch (addr) {

		// VDC Status Register
		case 0x0000: 
			return (vdcStatus);
		break;

		// VDC Data LSB
		case 0x0002:
			return (vdcDataL);
		break;

		// VDC Data MSB
		case 0x0003:
			return (vdcDataM);
		break;

		default:
			std::cout << "VDC Read ERROR" << std::endl;
			printf("Addr: 0x%X\n", addr);
			exit(0);
		break;
	}


}

// SDL Functions/Stuff

void HuC6270::handleKeyboard() {
	while ( SDL_PollEvent (&e) != 0) {
		if (e.type == SDL_QUIT) {
			exit(0);
		} else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
				case SDLK_LEFT:

				break;

				case SDLK_RIGHT:

				break;

				case SDLK_ESCAPE:
					exit(0);
				break;

				default:

				break;
			}
		}
	}
}

void HuC6270::drawText(std::string text, int x, int y) {
	SDL_Surface* tSurface = TTF_RenderText_Solid (debugFont, text.c_str(), {0, 0, 0});


	if (tSurface == NULL) {
		std::cout << "Unable to render text !" << std::endl;
		return;
	}

	if (debugText != NULL) {
		SDL_DestroyTexture( debugText );
		debugText = NULL;
	}

	debugText = SDL_CreateTextureFromSurface ( mainRenderer, tSurface );


	if (debugText == NULL) {
		std::cout << "Unable to create texture from rendered text !" << std::endl;
		return;
	}

	textRect.x = x;
	textRect.y = y;
	textRect.h = tSurface->h;
	textRect.w = tSurface->w;

	SDL_FreeSurface (tSurface);

}

void HuC6270::renderScene() {
	SDL_RenderClear    (mainRenderer);
	
	SDL_SetRenderDrawColor( mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_RenderCopy( mainRenderer, debugText, NULL, &textRect );
	SDL_RenderPresent  (mainRenderer);
}