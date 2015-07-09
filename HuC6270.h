#ifndef HUC_6270_H
#define HUC_6270_H

#include <iostream>
#include <SDL2/SDL.h>

class HuC6270 {

public:
	HuC6270();
	~HuC6270();

	void initVideo();

private:
	SDL_Window* mainWindow;
	SDL_Renderer *mainRenderer;
};


#endif