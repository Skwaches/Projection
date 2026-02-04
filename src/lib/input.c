#include "init.h"

bool h_KEY = false;
bool j_KEY = false;
bool k_KEY = false;
bool l_KEY = false;

bool a_KEY = false;
bool s_KEY = false;
bool d_KEY = false;
bool w_KEY = false;

bool lShift_KEY = false;

void handleKeyDown(SDL_Keycode key){
	switch(key){
		case SDLK_H:
			h_KEY = true;
			break;
		case SDLK_J:
			j_KEY = true;
			break;
		case SDLK_K:
			k_KEY = true;
			break;
		case SDLK_L:
			l_KEY = true;
			break;
		case SDLK_LSHIFT:
			lShift_KEY = true;
			break;
		case SDLK_A:
			a_KEY = true;
			break;
		case SDLK_S:
			s_KEY = true;
			break;
		case SDLK_D:
			d_KEY = true;
			break;
		case SDLK_W:
			w_KEY = true;
			break;
		case SDLK_ESCAPE:
			running = false;
			break;
		default:
			break;
	}
}

void handleKeyUp(SDL_Keycode key){
	switch(key){
		case SDLK_H:
			h_KEY = false;
			break;
		case SDLK_J:
			j_KEY = false;
			break;
		case SDLK_K:
			k_KEY = false;
			break;
		case SDLK_L:
			l_KEY = false;
			break;
		case SDLK_LSHIFT:
			lShift_KEY = false;
			break;
		case SDLK_A:
			a_KEY = false;
			break;
		case SDLK_S:
			s_KEY = false;
			break;
		case SDLK_D:
			d_KEY = false;
			break;
		case SDLK_W:
			w_KEY = false;
			break;
		default:
			break;
	}
}
void input(void){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_EVENT_QUIT:
				running = false;
				break;
			case SDL_EVENT_KEY_DOWN:
				handleKeyDown(event.key.key);
				break;
			case SDL_EVENT_KEY_UP:
				handleKeyUp(event.key.key);
				break;
			default:
				break;
		}
	}
}

