#include "update.h"
#include "init.h"

//FPS
#define FPS 200
bool LIMIT_FPS = false; float CURRENT_FPS = FPS; float WAIT_TIME = 1000.0/FPS;

//Window
SDL_Point WINDOW_SIZE = {2560,1440};
SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_FULLSCREEN;
const char* WINDOW_TITLE = "Cartesian";

//Colors
SDL_Color BACKGROUND = {0, 0 , 0, SDL_ALPHA_OPAQUE};
SDL_Color lineColor = {255, 0, 0, SDL_ALPHA_OPAQUE};

// Coordinates
Camera MAIN_CAMERA = { 
	.position = {0,0,0},
	.focal = 1,
	.rotation= { 0, 0}
};

//// SPEEDS
//Used as radians/second
//Planes are xy, xz and zy
//Note 1 = /180 radians
#define DEGREE SDL_PI_D/180 
SDL_FPoint ANGULAR_VELOCITY = {DEGREE * 90, DEGREE*90};
// Speed is in pixels per second
Vector3 LINEAR_SPEED = { 19, 19, 19};

//Rendering 
int GLOBAL_MAX_POINTS = 262144 * 2; //2**19 //This should fit("Probably")
int GLOBAL_MAX_OBJECTS = 10;

//Test Objects
Cuboid testCUBOID = { 
	.topBackLeft = {-0.6, 0.2, 10},
	.dimensions = {3, 3, 3} 
};

Circle testCIRCLE = {
	.center = {0,0.5,3},
	.accuracy = 100,
	.radius = 1 ,
	.color = {1, 1, 1, 0},
	.origin = {0,0,3}
};

Sphere testSPHERE = {
	.center = {0, 0, 3},
	.accuracy = { 800, 100},
	.radius = 2,
	.color = {255, 0, 0, 1}
};

Light testLIGHT = {
	.power = 0,
	.position = {0, 0, 0},
	.color = {1, 1, 1, 0}
};

//It is called with the init function.
bool frameOne(void){
	SDL_CHECK(SDL_GetWindowSize(window, &WINDOW_SIZE.x, &WINDOW_SIZE.y));
	MAIN_CAMERA.cosSin1 = (SDL_FPoint){ SDL_cosf(MAIN_CAMERA.rotation.x), SDL_sinf(MAIN_CAMERA.rotation.x)};
	MAIN_CAMERA.cosSin2 = (SDL_FPoint){ SDL_cosf(MAIN_CAMERA.rotation.y), SDL_sinf(MAIN_CAMERA.rotation.y)};
	initSphere(&testSPHERE);
	return true;
}

void frameLast(void){
	SDL_Log("%d points were used!", GLOBAL_FREE_INDEX);
	SDL_Log("Freeing memory...");
}
