%:include <SDL3/SDL.h>
%:include "update.h"
%:include "init.h"


int main(int argc, char** argv){
	init();
	while(RUNNING){
		input();
		update();
		render();
	}
	quit();
	return 0;
}
