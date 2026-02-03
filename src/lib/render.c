#include "init.h"

bool setColor(SDL_Color color){
	return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

bool render(){
	setColor(BACKGROUND);
	SDL_RenderClear(renderer);

	setColor(lineColor);
	SDL_RenderLines(renderer, testCUBOID.projection, CUBOID_PATH);

	SDL_RenderPresent(renderer);
	return true;
}

