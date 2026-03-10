#ifndef CLASSES
#define CLASSES
#include <SDL3/SDL.h>

// If this will be used for coordinates.
// Values for x and y should be between -1 to 1
typedef struct Vector3{
	float x,y,z;
}Vector3;

#define CUBOID_VERTICES_NO 8
#define CUBOID_EDGES_NO 12
#define FACE_POINTS_NO 4
#define LINE_PATH_TRIANGLE_NO 6

//These are the pair of all the indices of the edges of a cuboid
#define CUBOID_EDGES (int[CUBOID_EDGES_NO][2]){ {0, 1}, {1, 2}, {2, 3}, {3, 0},	{4, 5}, {5, 6}, {6, 7}, {7, 4},	 {0, 4}, {1, 5}, {2, 6}, {3, 7} } 

//These are the point that a face consists of
//Round the face, anti clockwise
//Original position is on the right
//Magic numbers galore
#define CUBOID_FACE_0 (int[FACE_POINTS_NO]){0, 1, 2, 3} //Back
#define CUBOID_FACE_1 (int[FACE_POINTS_NO]){4, 5, 6, 7} //Front
#define CUBOID_FACE_2 (int[FACE_POINTS_NO]){7, 6, 2, 3} //Right
#define CUBOID_FACE_3 (int[FACE_POINTS_NO]){0, 1, 5, 4} //Left
#define CUBOID_FACE_4 (int[FACE_POINTS_NO]){0, 4, 7, 3} //Top
#define CUBOID_FACE_5 (int[FACE_POINTS_NO]){1, 5, 6, 2} //Bottom
 
//How to follow points to render a 4 sided face with 2 triangles.
//This works if the point are arranged clockwise or anti-clockwise.
//These are the indices of the points.
#define FACE_ORDER (int[6]){0, 1, 2, 0, 2, 3}

typedef struct Cuboid {
	//The dimesions of the cuboid
	//This isn't used anywhere other than helping to calculate the original vertice positions
	Vector3 topBackLeft;
	Vector3 dimensions;
	
	//The center of the cuboid
	Vector3 center;

	//This is the projection of the vertices in screen coordinates
	SDL_Vertex *projection;
	Vector3* points;

}Cuboid;

typedef struct Circle{
	float radius;
	int accuracy;//Number of points to use;
	Vector3 center; 

	Vector3 *points;
	SDL_Vertex* projections;
	
}Circle;

typedef struct Sphere{
	//This is the memory address on the GLOBAL_POINTS where the sphere points start.
	//The first 2 items are the poles!
	Vector3* points;

	// The concentric circles
	// The poles
	Circle* circles;

	//Total number of points on the sphere.
	int count;

	// x -> Number of concentric circles above the equator. (It is duplicated at the bottom and an equator is provided.)
	// y -> Number of points on each circle.
	SDL_Point accuracy;

	// The center of the sphere is tracked and should be kept up to date.
	Vector3 center;

	// Radius. This is only used when initialising the sphere.
	// Changing it after initialisation will have no effect.
	// Changes are only registered if the sphere is reinitialised. (Don't recommend it at the moment)
	float radius;
}Sphere;

#endif
