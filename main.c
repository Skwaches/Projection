#include <SDL3/SDL.h>
#include "update.h"


#define FPS 60
#define WAIT_TIME (Uint64)(1000/FPS)
#define LIMIT_FPS true

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
