#include <SDL3/SDL.h>
// If this will be used for coordinates.
// Values for x and y should be between -1 to 1
typedef struct Vector3 {
	float x,y,z;
}Vector3;

#define CUBOID_VERTICES_NO 8
#define CUBOID_EDGES_NO 12
#define CUBOID_PATH_MESH_NO 16
#define CUBOID_PATH_TRIANGLE_NO 36
#define LINE_VERTICES_NO 4 
#define LINE_PATH_TRIANGLE_NO 6

//This is the order of indices to follow the vertices in order to trace the entire cuboid
#define CUBOID_PATH_MESH_INDICES     {0, 1, 2, 3, 0, 4, 5, 6, 7, 4, 7, 3, 2, 6, 5, 1}

//These are the pair of all the indices of the edges of a cuboid
#define CUBOID_EDGES (int[CUBOID_EDGES_NO][2]){ {0, 1}, {1, 2}, {2, 3}, {3, 0},	{4, 5}, {5, 6}, {6, 7}, {7, 4},	 {0, 4}, {1, 5}, {2, 6}, {3, 7} } 

//This is the order of indices to follow to form a full cuboid with triangle
#define CUBOID_PATH_TRIANGLE_INDICES (int[CUBOID_PATH_TRIANGLE_NO]){0, 1, 3, 1, 2, 3, 4, 5, 7, 5, 6, 7, 7, 6, 3, 6, 2, 3, 4, 5, 1, 4, 1, 0, 4, 0, 7, 0, 3, 7, 1, 5, 6, 1, 6, 2}

//This is the order of indices to follow to form a rectangle with 4 points.
//Indexing is from the top left anti-clockwise
#define LINE_PATH_TRIANGLE_INDICES (int[LINE_PATH_TRIANGLE_NO]){0, 1, 3, 1, 2, 3}

typedef struct Cuboid {
	
	//The topbackleft of the cuboid
	Vector3 topBackLeft;

	//The dimesions of the cuboid
	//This isn't used anywhere other than helping to calculate the original vertice positions
	Vector3 dimensions;
	
	//The corners of a cuboid
	Vector3 vertices[CUBOID_VERTICES_NO];

	//The center of the cuboid
	Vector3 center;

	//This is the projection of the vertices in screen coordinates
	SDL_FPoint projection[CUBOID_VERTICES_NO];

}Cuboid;

typedef struct Line{
	//The points to join
	SDL_FPoint a,b;
	SDL_FColor color;
	
	//The thickness of the line
	float thickness;
	SDL_Vertex vertexes[4];

}Line;
