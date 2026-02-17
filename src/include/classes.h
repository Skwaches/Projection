#include <SDL3/SDL.h>
// If this will be used for coordinates.
// Values for x and y should be between -1 to 1
typedef struct Vector3 {
	float x,y,z;
}Vector3;

#define CUBOID_VERTICES_NO 8
#define CUBOID_EDGES_NO 12
#define LINE_VERTICES_NO 4 
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
 
//How to follow points to render a face with  2 triangles
#define FACE_ORDER (int[6]){0, 1, 2, 0, 2, 3}

//This is the order of indices to follow to form a rectangle with 4 points.
//Indexing is from the top left anti-clockwise
#define LINE_PATH_TRIANGLE_INDICES (int[LINE_PATH_TRIANGLE_NO]){0, 1, 3, 1, 2, 3}

typedef struct Cuboid {
	
	//The dimesions of the cuboid
	//This isn't used anywhere other than helping to calculate the original vertice positions
	Vector3 topBackLeft;
	Vector3 dimensions;
	
	//The corners of a cuboid
	Vector3 vertices[CUBOID_VERTICES_NO];

	//The center of the cuboid
	Vector3 center;

	//This is the projection of the vertices in screen coordinates
	SDL_FPoint projection[CUBOID_VERTICES_NO];

	//Color. This is the colour of every face
}Cuboid;

typedef struct Circle{
	float radius;
	int accuracy;//number of points to use;
	Vector3 center, *points;
	SDL_FPoint* projection;
}Circle;

typedef struct Sphere{
	Circle* circles;
	SDL_Point accuracy; //Number of circles, Number of points on each circle
	float radius;
	Vector3 center;
}Sphere;

typedef struct Line{
	//The points to join
	SDL_FPoint a,b;
	SDL_FColor color;
	
	//The thickness of the line
	float thickness;
	SDL_Vertex vertexes[4];
}Line;

