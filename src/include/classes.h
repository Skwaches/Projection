#include <SDL3/SDL.h>
// If this will be used for coordinates.
// Values for x and y should be between -1 to 1
typedef struct Vector3 {
	float x,y,z;
}Vector3;

#define CUBOID_VERTICES 8
#define CUBOID_PATH 16
typedef struct Cuboid {
	//These are used to initialised the points. 
	Vector3 topBackLeft;
	Vector3 dimensions;

	//These are the created by functions
	Vector3 vertices[CUBOID_VERTICES];
	Vector3 path[CUBOID_PATH];
	Vector3 center;

	//This is the projection of the path in screen coordinates
	SDL_FPoint projection[CUBOID_PATH];

}Cuboid;

