#include "update.h"
#include "init.h"

//FPS
#define FPS 200
bool LIMIT_FPS = true;
float CURRENT_FPS = FPS;
float WAIT_TIME = 1000.0/FPS;

//Window
SDL_Point WINDOW_SIZE = {2560,1440};
SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_FULLSCREEN;
const char* WINDOW_TITLE = "Cartesian";

//Colors
SDL_Color BACKGROUND = {0, 0 , 0, SDL_ALPHA_OPAQUE};
SDL_Color lineColor = {230, 77, 228, SDL_ALPHA_OPAQUE};

// Coordinates
Vector3 USER_CAMERA = {0, 0, 0};

//// SPEEDS
//Used as radians/second
//Planes are xy, xz and zy
//Note 1 = /180 radians
#define DEGREE SDL_PI_D/180 
SDL_FPoint ANGULAR_VELOCITY = {DEGREE * 90, DEGREE*90};
// Speed is in pixels per second
Vector3 LINEAR_SPEED = { 9, 9, 9};

//Rendering 
int GLOBAL_MAX_ITEMS = 32768; //2**15 //This should fit("Probably")
int GLOBAL_FREE_ID = 0; //This is the index of the first available free spot. No item is at this index

//Test Objects
Cuboid testCUBOID = { 
	.topBackLeft = {-0.6, 0.2, 10},
	.dimensions = {3, 3, 3} 
};

Circle testCIRCLE = {
	.center = { 0, 0, 20},
	.accuracy = 10,
	.radius = 5 
};

Sphere testSPHERE = {
	.center = {0, 0, 5},
	.accuracy = { 20, 100 },
	.radius = 2
};

//It is called with the init function.
bool frameOne(void){
	GLOBAL_POINTS = SDL_malloc(GLOBAL_MAX_ITEMS * sizeof(Vector3));
	GLOBAL_PROJECTIONS = SDL_malloc(GLOBAL_MAX_ITEMS * sizeof(SDL_Vertex));
	if(!GLOBAL_POINTS || !GLOBAL_PROJECTIONS)
		return false;
	
	SDL_GetWindowSize(window, &WINDOW_SIZE.x, &WINDOW_SIZE.y);
	initCuboid(&testCUBOID);
	initCircle(&testCIRCLE);
	initSphere(&testSPHERE);
	return true;
}

void frameLast(void){
	SDL_free(testCIRCLE.points);
	SDL_free(testCIRCLE.projection);
	destroySphere(&testSPHERE);
}
