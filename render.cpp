#include "render.h"

render::render() {
   mainWindow   = NULL;
   mainRenderer = NULL;
   debugFont    = NULL;
}

render::~render() {
	SDL_DestroyRenderer (mainRenderer);
	SDL_DestroyWindow   (mainWindow);
	SDL_DestroyTexture  (debugText);

	mainRenderer = NULL;
	mainWindow   = NULL;
	debugText    = NULL;

	TTF_Quit();
	SDL_Quit();
}

bool render::initVideo() {
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

	debugFont = TTF_OpenFont("data/kkberkbm.ttf", 16);

	if (debugFont == NULL) {
		std::cout << "Error on TTF_OpenFont, SDL_Error:" << TTF_GetError() << std::endl;
		return false;
	}

	return true;
}


// SDL Functions/Stuff

void render::handleKeyboard() {
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

void render::waitPressButton() {

}

void render::drawText(std::string text, int x, int y) {
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

void render::renderScene() {
	SDL_RenderClear    (mainRenderer);
	
	SDL_SetRenderDrawColor( mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_RenderCopy( mainRenderer, debugText, NULL, &textRect );
	SDL_RenderPresent  (mainRenderer);
}