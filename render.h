#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define SCREEN_WIDTH  700
#define SCREEN_HEIGHT 512

class render {
public:
	render();
   ~render();

   bool initVideo();

   // Temporary
   void handleKeyboard();
   void waitPressButton();
   
   void drawText(std::string text, int x, int y);
   void renderScene();

private:

	SDL_Event e;
	SDL_Window* mainWindow;
	SDL_Renderer *mainRenderer;
    TTF_Font     *debugFont;
    SDL_Texture*  debugText;
    SDL_Rect      textRect;


};

#endif