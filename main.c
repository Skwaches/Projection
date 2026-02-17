#include <SDL3/SDL.h>
#include "update.h"


int main(int argc, char** argv){
	RUNNING = init();
	while(RUNNING){
		input();
		update();
		render();
	}
	quit();
	return 0;
}
