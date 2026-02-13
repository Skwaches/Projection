#include "update.h"


bool LIMIT_FPS = false;
#define FPS 200
float WAIT_TIME = 1000.0/FPS;

SDL_Point WINDOW_SIZE = {1920, 1080};
SDL_Color BACKGROUND = {0, 0 , 0, SDL_ALPHA_OPAQUE};
SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_FULLSCREEN;
const char* WINDOW_TITLE = "Cartesian";
SDL_Color lineColor = {230, 77, 228, SDL_ALPHA_OPAQUE};

//Used as radians/second
//Planes are xy, xz and zy
//Note 1 = /180 radians
#define DEGREE SDL_PI_D/180 
Vector3 ANGULAR_VELOCITY = {DEGREE * 90, DEGREE*90, DEGREE*90};

// Speed is in pixels per second
Vector3 LINEAR_SPEED = { 9, 9, 9};

//This is the cube definition
Cuboid testCUBOID = { 
	.topBackLeft = {-0.6, 0.2, 10},
	.dimensions = {3, 3, 3} 
};

Circle testCIRCLE = {
	.center = { 0, 0, 20},
	.accuracy = 1000,
	.radius = 5
};

Sphere testSPHERE = {
	.center = {0, 0, 20},
	.accuracy = {10, 500},
	.radius = 3
};

void frameOne(void){
	//This initialises all the other values
	//It is called with the init function.
	initCuboid(&testCUBOID);
	initCircle(&testCIRCLE);
	initSphere(&testSPHERE);
}

void frameLast(void){
	SDL_free(testCIRCLE.points);
	SDL_free(testCIRCLE.projection);
	destroySphere(&testSPHERE);
}
