#include "init.h"

bool running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool init(void){
	if(!SDL_Init(SDL_INIT_VIDEO))
		return false;
	if(!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_SIZE.x, WINDOW_SIZE.y, WINDOW_FLAGS, &window, &renderer))
		return false;
	frameOne();
	return true;
}

void quit(){
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	SDL_Log("Window has been closed successfully.");
}

