#include "init.h"

bool RUNNING = true;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Vector3* GLOBAL_POINTS = NULL;
SDL_Vertex* GLOBAL_PROJECTIONS = NULL;

inline void SDL_CHECK(bool success){
	if(!success){
		SDL_Log("Runtime Error! , %s:%i %s\n", __FILE__, __LINE__, SDL_GetError());
		RUNNING = false;
	};
};

void init(void){
	SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
	SDL_CHECK(SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_SIZE.x, WINDOW_SIZE.y, WINDOW_FLAGS, &window, &renderer));
	frameOne();
}

void quit(){
	frameLast();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	SDL_Log("Window has been closed successfully.");
}

