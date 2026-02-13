#include <SDL3/SDL.h>
#include "update.h"


int main(int argc, char** argv){
	running = init();
	while(running){
		input();
		update();
		render();
	}
	quit();
	return 0;
}
