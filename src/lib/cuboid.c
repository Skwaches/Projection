#include "update.h"

// Creates the front and back faces of a cube when given the "back-left" point 
void vertexes(Vector3 vertices[CUBOID_VERTICES], Vector3 bak, Vector3 dimensions){
	//Back Face
	vertices[0] = bak;
	vertices[1] = (Vector3){bak.x, 				  bak.y - dimensions.y, bak.z};
	vertices[2] = (Vector3){bak.x + dimensions.x, bak.y - dimensions.y, bak.z};
	vertices[3] = (Vector3){bak.x + dimensions.x, bak.y,				bak.z};
	
	//Front Face
	vertices[4] = (Vector3){bak.x, 				  bak.y, 				bak.z + dimensions.z};
	vertices[5] = (Vector3){bak.x, 				  bak.y - dimensions.y, bak.z + dimensions.z};
	vertices[6] = (Vector3){bak.x + dimensions.x, bak.y - dimensions.y, bak.z + dimensions.z};
	vertices[7] = (Vector3){bak.x + dimensions.x, bak.y,				bak.z + dimensions.z};
}

//Returns the center of the cube provided 
Vector3 centerCuboid(Cuboid cube){
	Vector3 center = { 
	(cube.vertices[6].x + cube.vertices[0].x)/2 , 
	(cube.vertices[6].y + cube.vertices[0].y)/2 ,
	(cube.vertices[6].z + cube.vertices[0].z)/2 , 
		};
	return center;
}
// Returns the path needed to follow in order to render a cuboid.
// FIXME This path may not be optimal. Switching to edges may be smarter. Do this!
void linePath(Vector3 path[CUBOID_PATH],Vector3 vertices[CUBOID_VERTICES]){
	int indexer = 0;

	for(int i = 0; i < 4; i++)
		path[indexer++] = vertices[i];
	path[indexer++] = vertices[0];

	for(int i = 4; i < 8; i++)
		path[indexer++] = vertices[i];
	path[indexer++] = vertices[4];

	path[indexer++] = vertices[7];
	path[indexer++] = vertices[3];
	path[indexer++] = vertices[2];
	path[indexer++] = vertices[6];
	path[indexer++] = vertices[5];
	path[indexer]   = vertices[1];
}

void updateCuboid(Cuboid* cube){
	linePath(cube->path, cube->vertices);
	cube->center = centerCuboid(*cube);
	for(int i = 0; i < CUBOID_PATH; i++){
		SDL_FPoint projected = projection(cube->path[i]);
		cube->projection[i] = convertScreen(projected);
	}
}
void initCuboid(Cuboid* cube){
	vertexes(cube->vertices, cube->topBackLeft, cube->dimensions);
	updateCuboid(cube);
}

